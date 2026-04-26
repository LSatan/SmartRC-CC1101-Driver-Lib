/*
  SmartRC_CC1101.cpp - Advanced CC1101 module library (V3.0.0)
*/
#include "SmartRC_CC1101.h"
#include <math.h>
#include <Arduino.h>
#define WRITE_BURST     0x40
#define READ_SINGLE     0x80
#define READ_BURST      0xC0
#define BYTES_IN_RXFIFO 0x7F

// CENTRAL QUARTZ DEFINITION (Can be changed by the user for 27MHz crystals)
#define F_OSC           26000000.0f 

#define CC1101_SPI_SETTINGS SPISettings(4000000, MSBFIRST, SPI_MODE0)

const uint8_t PA_TABLE_315[8]  = {0x12,0x0D,0x1C,0x34,0x51,0x85,0xCB,0xC2};
const uint8_t PA_TABLE_433[8]  = {0x12,0x0E,0x1D,0x34,0x60,0x84,0xC8,0xC0};
const uint8_t PA_TABLE_868[10] = {0x03,0x17,0x1D,0x26,0x37,0x50,0x86,0xCD,0xC5,0xC0};
const uint8_t PA_TABLE_915[10] = {0x03,0x0E,0x1E,0x27,0x38,0x8E,0x84,0xCC,0xC3,0xC0};

// --- Global instances ---
// We create a single object in memory under the new name.
SmartRC_CC1101 SmartRC_cc1101;

// The old name "ELECHOUSE_cc1101" is now simply a reference to exactly the same object!
// This way we don't waste any RAM for old scripts.
SmartRC_CC1101& ELECHOUSE_cc1101 = SmartRC_cc1101;

// --- Internal Helper Functions ---

bool SmartRC_CC1101::WaitMiso(uint16_t timeout_ms) {
    uint32_t start = millis();
    while (digitalRead(MISO_PIN)) {
        if (millis() - start > timeout_ms) return false;
    }
    return true;
}

void SmartRC_CC1101::SpiStart(void) {
    SPI.beginTransaction(CC1101_SPI_SETTINGS);
    digitalWrite(SS_PIN, LOW);
}

void SmartRC_CC1101::SpiEnd(void) {
    digitalWrite(SS_PIN, HIGH);
    SPI.endTransaction();
}

void SmartRC_CC1101::GDO_Set(void) {
    pinMode(GDO0, OUTPUT);
    pinMode(GDO2, INPUT);
}

void SmartRC_CC1101::GDO0_Set(void) {
    pinMode(GDO0, INPUT);
}

// --- SPI Basis-Operationen ---

byte SmartRC_CC1101::SpiReadStatus(byte addr) {
    byte value = 0;
    SpiStart();
    if (WaitMiso()) {
        SPI.transfer(addr | READ_BURST);
        value = SPI.transfer(0);
    }
    SpiEnd();
    return value;
}

byte SmartRC_CC1101::SpiReadReg(byte addr) {
    byte value = 0;
    SpiStart();
    if (WaitMiso()) {
        SPI.transfer(addr | READ_SINGLE);
        value = SPI.transfer(0);
    }
    SpiEnd();
    return value;
}

void SmartRC_CC1101::SpiWriteReg(byte addr, byte value) {
    SpiStart();
    if (WaitMiso()) {
        SPI.transfer(addr);
        SPI.transfer(value);
    }
    SpiEnd();
}

void SmartRC_CC1101::SpiStrobe(byte strobe) {
    SpiStart();
    if (WaitMiso()) {
        SPI.transfer(strobe);
    }
    SpiEnd();
}

void SmartRC_CC1101::SpiWriteBurstReg(byte addr, byte *buffer, byte num) {
    SpiStart();
    if (WaitMiso()) {
        SPI.transfer(addr | WRITE_BURST);
        for (byte i = 0; i < num; i++) SPI.transfer(buffer[i]);
    }
    SpiEnd();
}

void SmartRC_CC1101::SpiReadBurstReg(byte addr, byte *buffer, byte num) {
    SpiStart();
    if (WaitMiso()) {
        SPI.transfer(addr | READ_BURST);
        for (byte i = 0; i < num; i++) buffer[i] = SPI.transfer(0);
    }
    SpiEnd();
}

// --- Setup and Configuration ---

void SmartRC_CC1101::setSpi(void) {
    if (spi_initialized || custom_spi_pins) return;
    #if defined __AVR_ATmega168__ || defined __AVR_ATmega328P__
        SCK_PIN = 13; MISO_PIN = 12; MOSI_PIN = 11; SS_PIN = 10;
    #elif defined __AVR_ATmega1280__ || defined __AVR_ATmega2560__
        SCK_PIN = 52; MISO_PIN = 50; MOSI_PIN = 51; SS_PIN = 53;
    #elif defined ESP8266
        SCK_PIN = 14; MISO_PIN = 12; MOSI_PIN = 13; SS_PIN = 15;
    #elif defined ESP32
        SCK_PIN = 18; MISO_PIN = 19; MOSI_PIN = 23; SS_PIN = 5;
    #else
        SCK_PIN = 13; MISO_PIN = 12; MOSI_PIN = 11; SS_PIN = 10;
    #endif
}

void SmartRC_CC1101::setSpiPin(byte sck, byte miso, byte mosi, byte ss) {
    SCK_PIN = sck; MISO_PIN = miso; MOSI_PIN = mosi; SS_PIN = ss;
	custom_spi_pins = true;
}

void SmartRC_CC1101::setGDO(byte gdo0, byte gdo2) {
    GDO0 = gdo0; GDO2 = gdo2;
    GDO_Set();
}

void SmartRC_CC1101::setGDO0(byte gdo0) {
    GDO0 = gdo0;
    GDO0_Set();
}

void SmartRC_CC1101::Init(void) {
    setSpi();
    
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);
    pinMode(SS_PIN, OUTPUT);

    if (!spi_initialized) {
        #ifdef ESP32
        SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
        #else
        SPI.begin();
        #endif
        spi_initialized = true;
    }
    digitalWrite(SS_PIN, HIGH);
    digitalWrite(SCK_PIN, HIGH);
    digitalWrite(MOSI_PIN, LOW);
    
    Reset();
    RegConfigSettings();
}

void SmartRC_CC1101::Reset(void) {
    digitalWrite(SS_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(SS_PIN, HIGH);
    delayMicroseconds(40);
    digitalWrite(SS_PIN, LOW);
    if(WaitMiso()) {
        SpiStart();
        SPI.transfer(CC1101_SRES);
        SpiEnd();
    }
    WaitMiso();
    digitalWrite(SS_PIN, HIGH);
}

void SmartRC_CC1101::Calibrate(void) {
    if (MHz >= 300 && MHz <= 348) {
        SpiWriteReg(CC1101_FSCTRL0, map(MHz, 300, 348, clb1[0], clb1[1]));
        if (MHz < 322.88) { SpiWriteReg(CC1101_TEST0, 0x0B); }
        else {
            SpiWriteReg(CC1101_TEST0, 0x09);
            int s = SpiReadStatus(CC1101_FSCAL2);
            if (s < 32) SpiWriteReg(CC1101_FSCAL2, s + 32);
            if (last_pa != 1) setPA(pa);
        }
    } else if (MHz >= 378 && MHz <= 464) {
        SpiWriteReg(CC1101_FSCTRL0, map(MHz, 378, 464, clb2[0], clb2[1]));
        if (MHz < 430.5) { SpiWriteReg(CC1101_TEST0, 0x0B); }
        else {
            SpiWriteReg(CC1101_TEST0, 0x09);
            int s = SpiReadStatus(CC1101_FSCAL2);
            if (s < 32) SpiWriteReg(CC1101_FSCAL2, s + 32);
            if (last_pa != 2) setPA(pa);
        }
    } else if (MHz >= 779 && MHz <= 899.99) {
        SpiWriteReg(CC1101_FSCTRL0, map(MHz, 779, 899, clb3[0], clb3[1]));
        if (MHz < 861) { SpiWriteReg(CC1101_TEST0, 0x0B); }
        else {
            SpiWriteReg(CC1101_TEST0, 0x09);
            int s = SpiReadStatus(CC1101_FSCAL2);
            if (s < 32) SpiWriteReg(CC1101_FSCAL2, s + 32);
            if (last_pa != 3) setPA(pa);
        }
    } else if (MHz >= 900 && MHz <= 928) {
        SpiWriteReg(CC1101_FSCTRL0, map(MHz, 900, 928, clb4[0], clb4[1]));
        SpiWriteReg(CC1101_TEST0, 0x09);
        int s = SpiReadStatus(CC1101_FSCAL2);
        if (s < 32) SpiWriteReg(CC1101_FSCAL2, s + 32);
        if (last_pa != 4) setPA(pa);
    }
}

void SmartRC_CC1101::setClb(byte b, byte s, byte e) {
    if (b == 1) { clb1[0] = s; clb1[1] = e; }
    else if (b == 2) { clb2[0] = s; clb2[1] = e; }
    else if (b == 3) { clb3[0] = s; clb3[1] = e; }
    else if (b == 4) { clb4[0] = s; clb4[1] = e; }
}

void SmartRC_CC1101::RegConfigSettings(void) {
    SpiWriteReg(CC1101_FSCTRL1,  0x06);
    setCCMode(ccmode);
    setMHZ(MHz); 
    SpiWriteReg(CC1101_MDMCFG1,  0x02);
    SpiWriteReg(CC1101_MDMCFG0,  0xF8);
    SpiWriteReg(CC1101_CHANNR,   chan);
    SpiWriteReg(CC1101_DEVIATN,  0x47);
    SpiWriteReg(CC1101_FREND1,   0x56);
    SpiWriteReg(CC1101_MCSM0 ,   0x18); 
    SpiWriteReg(CC1101_FOCCFG,   0x16);
    SpiWriteReg(CC1101_BSCFG,    0x1C);
    SpiWriteReg(CC1101_AGCCTRL2, 0xC7);
    SpiWriteReg(CC1101_AGCCTRL1, 0x00);
    SpiWriteReg(CC1101_AGCCTRL0, 0xB2);
    SpiWriteReg(CC1101_FSCAL3,   0xE9);
    SpiWriteReg(CC1101_FSCAL2,   0x2A);
    SpiWriteReg(CC1101_FSCAL1,   0x00);
    SpiWriteReg(CC1101_FSCAL0,   0x1F);
    SpiWriteReg(CC1101_FSTEST,   0x59);
    SpiWriteReg(CC1101_TEST2,    0x81);
    SpiWriteReg(CC1101_TEST1,    0x35);
    SpiWriteReg(CC1101_TEST0,    0x09);
    SpiWriteReg(CC1101_PKTCTRL1, 0x04);
    SpiWriteReg(CC1101_ADDR,     0x00);
    SpiWriteReg(CC1101_PKTLEN,   0x00);
}

// --- Getter & Setter ---

bool SmartRC_CC1101::getCC1101(void) {
    byte version = SpiReadStatus(CC1101_VERSION);
    return (version > 0 && version < 255);
}

void SmartRC_CC1101::setMHZ(float mhz) {
    MHz = mhz;
    uint32_t freq = (uint32_t)(mhz * 65536.0f / (F_OSC / 1000000.0f));
    SpiWriteReg(CC1101_FREQ2, (freq >> 16) & 0xFF);
    SpiWriteReg(CC1101_FREQ1, (freq >> 8) & 0xFF);
    SpiWriteReg(CC1101_FREQ0, freq & 0xFF);
    Calibrate();
}

float SmartRC_CC1101::getMHZ(void) {
    uint32_t freq = ((uint32_t)SpiReadReg(CC1101_FREQ2) << 16) |
                    ((uint32_t)SpiReadReg(CC1101_FREQ1) << 8) |
                    ((uint32_t)SpiReadReg(CC1101_FREQ0));
    return (float)freq * (F_OSC / 1000000.0f) / 65536.0f;
}

void SmartRC_CC1101::setDRate(float d) {
    float baud = d * 1000.0f;
    if (baud > 600000) baud = 600000;
    if (baud < 600) baud = 600;

    byte drate_e = (byte)(log(baud * 268435456.0f / (F_OSC * 256.0f)) / log(2.0f));
    byte drate_m = (byte)((baud * 268435456.0f) / (F_OSC * pow(2, drate_e)) - 256 + 0.5f);
    
    byte current4 = SpiReadStatus(CC1101_MDMCFG4) & 0xF0; 
    SpiWriteReg(CC1101_MDMCFG4, current4 | (drate_e & 0x0F));
    SpiWriteReg(CC1101_MDMCFG3, drate_m);
}

float SmartRC_CC1101::getDRate(void) {
    byte drate_e = SpiReadReg(CC1101_MDMCFG4) & 0x0F;
    byte drate_m = SpiReadReg(CC1101_MDMCFG3);
    return ((256.0f + drate_m) * pow(2, drate_e) * F_OSC) / 268435456.0f / 1000.0f;
}

void SmartRC_CC1101::setRxBW(float f) {
    float target = f * 1000.0f;
    float ratio = F_OSC / (8.0f * target);
    
    // AVR Safe Logarithm
    int e = (int)(log(ratio / 4.0f) / log(2.0f));
    if (e < 0) e = 0;
    if (e > 3) e = 3;
    
    int m = (int)roundf((ratio / pow(2, e)) - 4.0f);
    if (m < 0) m = 0;
    if (m > 3) m = 3;
    
    byte current4 = SpiReadStatus(CC1101_MDMCFG4) & 0x0F;
    SpiWriteReg(CC1101_MDMCFG4, ((byte)e << 6) | ((byte)m << 4) | current4);
}

float SmartRC_CC1101::getRxBW(void) {
    byte val = SpiReadReg(CC1101_MDMCFG4);
    byte bw_e = (val >> 6) & 0x03;
    byte bw_m = (val >> 4) & 0x03;
    return (F_OSC / (8.0f * (4.0f + bw_m) * pow(2, bw_e))) / 1000.0f;
}

void SmartRC_CC1101::setChsp(float f) {
    float chsp = f * 1000.0f; 
    byte chanspc_e = (byte)(log(chsp * 262144.0f / (F_OSC * 256.0f)) / log(2.0f));
    if (chanspc_e > 3) chanspc_e = 3;
    byte chanspc_m = (byte)((chsp * 262144.0f) / (F_OSC * pow(2, chanspc_e)) - 256 + 0.5f);

    byte current1 = SpiReadStatus(CC1101_MDMCFG1) & 0xFC;
    SpiWriteReg(CC1101_MDMCFG1, current1 | chanspc_e);
    SpiWriteReg(CC1101_MDMCFG0, chanspc_m);
}

float SmartRC_CC1101::getChsp(void) {
    byte chanspc_e = SpiReadReg(CC1101_MDMCFG1) & 0x03;
    byte chanspc_m = SpiReadReg(CC1101_MDMCFG0);
    return (F_OSC / 262144.0f * (256.0f + chanspc_m) * pow(2, chanspc_e)) / 1000.0f;
}

void SmartRC_CC1101::setDeviation(float d) {
    float target = d * 1000.0f;
    byte e = (byte)(log(target * 131072.0f / (F_OSC * 8.0f)) / log(2.0f));
    if (e > 7) e = 7;
    byte m = (byte)((target * 131072.0f) / (F_OSC * pow(2, e)) - 8 + 0.5f);
    if (m > 7) m = 7;
    
    SpiWriteReg(CC1101_DEVIATN, (e << 4) | m);
}

void SmartRC_CC1101::setCCMode(bool s) {
    ccmode = s;
    byte m4 = SpiReadStatus(CC1101_MDMCFG4) & 0xF0; 
    if (ccmode) {
        SpiWriteReg(CC1101_IOCFG2,      0x0B);
        SpiWriteReg(CC1101_IOCFG0,      0x06);
        SpiWriteReg(CC1101_PKTCTRL0,    0x05);
        SpiWriteReg(CC1101_MDMCFG3,     0xF8);
        SpiWriteReg(CC1101_MDMCFG4,     m4 | 11);
    } else {
        SpiWriteReg(CC1101_IOCFG2,      0x0D);
        SpiWriteReg(CC1101_IOCFG0,      0x0D);
        SpiWriteReg(CC1101_PKTCTRL0,    0x32);
        SpiWriteReg(CC1101_MDMCFG3,     0x93);
        SpiWriteReg(CC1101_MDMCFG4,     m4 | 7);
    }
    setModulation(modulation);
}

void SmartRC_CC1101::setModulation(byte m) {
    if (m > 4) m = 4;
    modulation = m;
    
    byte current2 = SpiReadStatus(CC1101_MDMCFG2) & ~0x70;
    byte frend0 = 0x10;
    byte mod_format = 0x00;

    switch (m) {
        case 0: mod_format = 0x00; frend0 = 0x10; break; 
        case 1: mod_format = 0x10; frend0 = 0x10; break; 
        case 2: mod_format = 0x30; frend0 = 0x11; break; 
        case 3: mod_format = 0x40; frend0 = 0x10; break; 
        case 4: mod_format = 0x70; frend0 = 0x10; break; 
    }
    
    SpiWriteReg(CC1101_MDMCFG2, current2 | mod_format);
    SpiWriteReg(CC1101_FREND0, frend0);
    setPA(pa);
}

void SmartRC_CC1101::setPA(int p) {
    int a = 0;
    pa = p;

    if (MHz >= 300 && MHz <= 348) {
        if (pa <= -30) a = PA_TABLE_315[0];
        else if (pa <= -20) a = PA_TABLE_315[1];
        else if (pa <= -15) a = PA_TABLE_315[2];
        else if (pa <= -10) a = PA_TABLE_315[3];
        else if (pa <= 0)   a = PA_TABLE_315[4];
        else if (pa <= 5)   a = PA_TABLE_315[5];
        else if (pa <= 7)   a = PA_TABLE_315[6];
        else a = PA_TABLE_315[7];
        last_pa = 1;
    } else if (MHz >= 378 && MHz <= 464) {
        if (pa <= -30) a = PA_TABLE_433[0];
        else if (pa <= -20) a = PA_TABLE_433[1];
        else if (pa <= -15) a = PA_TABLE_433[2];
        else if (pa <= -10) a = PA_TABLE_433[3];
        else if (pa <= 0)   a = PA_TABLE_433[4];
        else if (pa <= 5)   a = PA_TABLE_433[5];
        else if (pa <= 7)   a = PA_TABLE_433[6];
        else a = PA_TABLE_433[7];
        last_pa = 2;
    } else if (MHz >= 779 && MHz <= 899.99) {
        if (pa <= -30) a = PA_TABLE_868[0];
        else if (pa <= -20) a = PA_TABLE_868[1];
        else if (pa <= -15) a = PA_TABLE_868[2];
        else if (pa <= -10) a = PA_TABLE_868[3];
        else if (pa <= -6)  a = PA_TABLE_868[4];
        else if (pa <= 0)   a = PA_TABLE_868[5];
        else if (pa <= 5)   a = PA_TABLE_868[6];
        else if (pa <= 7)   a = PA_TABLE_868[7];
        else if (pa <= 10)  a = PA_TABLE_868[8];
        else a = PA_TABLE_868[9];
        last_pa = 3;
    } else if (MHz >= 900 && MHz <= 928) {
        if (pa <= -30) a = PA_TABLE_915[0];
        else if (pa <= -20) a = PA_TABLE_915[1];
        else if (pa <= -15) a = PA_TABLE_915[2];
        else if (pa <= -10) a = PA_TABLE_915[3];
        else if (pa <= -6)  a = PA_TABLE_915[4];
        else if (pa <= 0)   a = PA_TABLE_915[5];
        else if (pa <= 5)   a = PA_TABLE_915[6];
        else if (pa <= 7)   a = PA_TABLE_915[7];
        else if (pa <= 10)  a = PA_TABLE_915[8];
        else a = PA_TABLE_915[9];
        last_pa = 4;
    }
    
    uint8_t paTable[8] = {0,0,0,0,0,0,0,0};
    if (modulation == 2) {
        paTable[0] = 0; paTable[1] = a;
    } else {
        paTable[0] = a; paTable[1] = 0;
    }
    SpiWriteBurstReg(CC1101_PATABLE, paTable, 8);
}

void SmartRC_CC1101::setChannel(byte ch) {
    chan = ch;
    SpiWriteReg(CC1101_CHANNR, chan);
}

void SmartRC_CC1101::setSyncWord(byte sh, byte sl) {
    SpiWriteReg(CC1101_SYNC1, sh);
    SpiWriteReg(CC1101_SYNC0, sl);
}

void SmartRC_CC1101::setAddr(byte v) {
    SpiWriteReg(CC1101_ADDR, v);
}

void SmartRC_CC1101::setPQT(byte v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL1);
    val = (val & ~0xE0) | ((v & 0x07) << 5);
    SpiWriteReg(CC1101_PKTCTRL1, val);
}

void SmartRC_CC1101::setCRC_AF(bool v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL1);
    val = v ? (val | 0x08) : (val & ~0x08);
    SpiWriteReg(CC1101_PKTCTRL1, val);
}

void SmartRC_CC1101::setAppendStatus(bool v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL1);
    val = v ? (val | 0x04) : (val & ~0x04);
    SpiWriteReg(CC1101_PKTCTRL1, val);
}

void SmartRC_CC1101::setAdrChk(byte v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL1);
    val = (val & ~0x03) | (v & 0x03);
    SpiWriteReg(CC1101_PKTCTRL1, val);
}

void SmartRC_CC1101::setWhiteData(bool v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL0);
    val = v ? (val | 0x40) : (val & ~0x40);
    SpiWriteReg(CC1101_PKTCTRL0, val);
}

void SmartRC_CC1101::setPktFormat(byte v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL0);
    val = (val & ~0x30) | ((v & 0x03) << 4);
    SpiWriteReg(CC1101_PKTCTRL0, val);
}

void SmartRC_CC1101::setCrc(bool v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL0);
    val = v ? (val | 0x04) : (val & ~0x04);
    SpiWriteReg(CC1101_PKTCTRL0, val);
}

void SmartRC_CC1101::setLengthConfig(byte v) {
    byte val = SpiReadStatus(CC1101_PKTCTRL0);
    val = (val & ~0x03) | (v & 0x03);
    SpiWriteReg(CC1101_PKTCTRL0, val);
}

void SmartRC_CC1101::setPacketLength(byte v) {
    SpiWriteReg(CC1101_PKTLEN, v);
}

void SmartRC_CC1101::setDcFilterOff(bool v) {
    byte val = SpiReadStatus(CC1101_MDMCFG2);
    val = v ? (val | 0x80) : (val & ~0x80);
    SpiWriteReg(CC1101_MDMCFG2, val);
}

void SmartRC_CC1101::setManchester(bool v) {
    byte val = SpiReadStatus(CC1101_MDMCFG2);
    val = v ? (val | 0x08) : (val & ~0x08);
    SpiWriteReg(CC1101_MDMCFG2, val);
}

void SmartRC_CC1101::setSyncMode(byte v) {
    byte val = SpiReadStatus(CC1101_MDMCFG2);
    val = (val & ~0x07) | (v & 0x07);
    SpiWriteReg(CC1101_MDMCFG2, val);
}

void SmartRC_CC1101::setFEC(bool v) {
    byte val = SpiReadStatus(CC1101_MDMCFG1);
    val = v ? (val | 0x80) : (val & ~0x80);
    SpiWriteReg(CC1101_MDMCFG1, val);
}

void SmartRC_CC1101::setPRE(byte v) {
    byte val = SpiReadStatus(CC1101_MDMCFG1);
    val = (val & ~0x70) | ((v & 0x07) << 4);
    SpiWriteReg(CC1101_MDMCFG1, val);
}

// --- Transceiver States ---

void SmartRC_CC1101::SetTx(void) {
    SpiStrobe(CC1101_SIDLE);
    SpiStrobe(CC1101_STX);
    trxstate = 1;
}

void SmartRC_CC1101::SetRx(void) {
    SpiStrobe(CC1101_SIDLE);
    SpiStrobe(CC1101_SRX);
    trxstate = 2;
}

void SmartRC_CC1101::SetTx(float mhz) {
    SpiStrobe(CC1101_SIDLE);
    setMHZ(mhz);
    SpiStrobe(CC1101_STX);
    trxstate = 1;
}

void SmartRC_CC1101::SetRx(float mhz) {
    SpiStrobe(CC1101_SIDLE);
    setMHZ(mhz);
    SpiStrobe(CC1101_SRX);
    trxstate = 2;
}

int SmartRC_CC1101::getRssi(void) {
    int rssi = SpiReadStatus(CC1101_RSSI);
    if (rssi >= 128) rssi = (rssi - 256) / 2 - 74;
    else rssi = (rssi / 2) - 74;
    return rssi;
}

byte SmartRC_CC1101::getLqi(void) {
    return SpiReadStatus(CC1101_LQI);
}

void SmartRC_CC1101::setSres(void) {
    SpiStrobe(CC1101_SRES);
    trxstate = 0;
}

void SmartRC_CC1101::setSidle(void) {
    SpiStrobe(CC1101_SIDLE);
    trxstate = 0;
}

void SmartRC_CC1101::goSleep(void) {
    trxstate = 0;
    SpiStrobe(0x36); // SIDLE
    SpiStrobe(0x39); // SPWD
}

byte SmartRC_CC1101::getMode(void) {
    return trxstate;
}

// --- Transmit and receive functions ---

void SmartRC_CC1101::SendData(char *txchar) {
    SendData((byte*)txchar, strlen(txchar));
}

void SmartRC_CC1101::SendData(byte *txBuffer, byte size) {
    SpiWriteReg(CC1101_TXFIFO, size);
    SpiWriteBurstReg(CC1101_TXFIFO, txBuffer, size);
    SpiStrobe(CC1101_SIDLE);
    SpiStrobe(CC1101_STX);
    uint32_t start = millis();
    while (!digitalRead(GDO0) && (millis() - start < 500)); 
    start = millis();
    while (digitalRead(GDO0) && (millis() - start < 500));
    SpiStrobe(CC1101_SFTX);
    trxstate = 1;
}

void SmartRC_CC1101::SendData(char *txchar, int t) {
    SendData((byte*)txchar, strlen(txchar), t);
}

void SmartRC_CC1101::SendData(byte *txBuffer, byte size, int t) {
    SpiWriteReg(CC1101_TXFIFO, size);
    SpiWriteBurstReg(CC1101_TXFIFO, txBuffer, size);
    SpiStrobe(CC1101_SIDLE);
    SpiStrobe(CC1101_STX);
    delay(t);
    SpiStrobe(CC1101_SFTX);
    trxstate = 1;
}

bool SmartRC_CC1101::CheckCRC(void) {
    byte lqi = SpiReadStatus(CC1101_LQI);
    bool crc_ok = bitRead(lqi, 7);
    if (crc_ok) {
        return true;
    } else {
        SpiStrobe(CC1101_SFRX);
        SpiStrobe(CC1101_SRX);
        return false;
    }
}

bool SmartRC_CC1101::CheckRxFifo(int t) {
    if (trxstate != 2) SetRx();
    if (SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO) {
        delay(t);
        return true;
    } else {
        return false;
    }
}

byte SmartRC_CC1101::CheckReceiveFlag(void) {
    if (trxstate != 2) SetRx();
    if (digitalRead(GDO0)) {
        uint32_t start = millis();
        while (digitalRead(GDO0) && (millis() - start < 200));
        return 1;
    } else {
        return 0;
    }
}

byte SmartRC_CC1101::ReceiveData(byte *rxBuffer) {
    byte size;
    byte status[2];

    if (SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO) {
        size = SpiReadReg(CC1101_RXFIFO);
        SpiReadBurstReg(CC1101_RXFIFO, rxBuffer, size);
        SpiReadBurstReg(CC1101_RXFIFO, status, 2);
        SpiStrobe(CC1101_SFRX);
        SpiStrobe(CC1101_SRX);
        return size;
    } else {
        SpiStrobe(CC1101_SFRX);
        SpiStrobe(CC1101_SRX);
        return 0;
    }
}