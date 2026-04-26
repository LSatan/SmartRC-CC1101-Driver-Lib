/*
  SmartRC_CC1101.h - Advanced CC1101 module library
  Version: 3.0.0 (2026 Reboot)
  Author: Little Satan (SmartRC)
  Based on original work by: Wilson Shen (Elechouse)
*/
#ifndef SMARTRC_CC1101_H
#define SMARTRC_CC1101_H

#include <Arduino.h>
#include <SPI.h>

//***************************************CC1101 define**************************************************//
// CC1101 CONFIG REGSITER
#define CC1101_IOCFG2       0x00
#define CC1101_IOCFG1       0x01
#define CC1101_IOCFG0       0x02
#define CC1101_FIFOTHR      0x03
#define CC1101_SYNC1        0x04
#define CC1101_SYNC0        0x05
#define CC1101_PKTLEN       0x06
#define CC1101_PKTCTRL1     0x07
#define CC1101_PKTCTRL0     0x08
#define CC1101_ADDR         0x09
#define CC1101_CHANNR       0x0A
#define CC1101_FSCTRL1      0x0B
#define CC1101_FSCTRL0      0x0C
#define CC1101_FREQ2        0x0D
#define CC1101_FREQ1        0x0E
#define CC1101_FREQ0        0x0F
#define CC1101_MDMCFG4      0x10
#define CC1101_MDMCFG3      0x11
#define CC1101_MDMCFG2      0x12
#define CC1101_MDMCFG1      0x13
#define CC1101_MDMCFG0      0x14
#define CC1101_DEVIATN      0x15
#define CC1101_MCSM2        0x16
#define CC1101_MCSM1        0x17
#define CC1101_MCSM0        0x18
#define CC1101_FOCCFG       0x19
#define CC1101_BSCFG        0x1A
#define CC1101_AGCCTRL2     0x1B
#define CC1101_AGCCTRL1     0x1C
#define CC1101_AGCCTRL0     0x1D
#define CC1101_WOREVT1      0x1E
#define CC1101_WOREVT0      0x1F
#define CC1101_WORCTRL      0x20
#define CC1101_FREND1       0x21
#define CC1101_FREND0       0x22
#define CC1101_FSCAL3       0x23
#define CC1101_FSCAL2       0x24
#define CC1101_FSCAL1       0x25
#define CC1101_FSCAL0       0x26
#define CC1101_RCCTRL1      0x27
#define CC1101_RCCTRL0      0x28
#define CC1101_FSTEST       0x29
#define CC1101_PTEST        0x2A
#define CC1101_AGCTEST      0x2B
#define CC1101_TEST2        0x2C
#define CC1101_TEST1        0x2D
#define CC1101_TEST0        0x2E

//CC1101 Strobe commands
#define CC1101_SRES         0x30
#define CC1101_SFSTXON      0x31
#define CC1101_SXOFF        0x32
#define CC1101_SCAL         0x33
#define CC1101_SRX          0x34
#define CC1101_STX          0x35
#define CC1101_SIDLE        0x36
#define CC1101_SAFC         0x37
#define CC1101_SWOR         0x38
#define CC1101_SPWD         0x39
#define CC1101_SFRX         0x3A
#define CC1101_SFTX         0x3B
#define CC1101_SWORRST      0x3C
#define CC1101_SNOP         0x3D

//CC1101 STATUS REGSITER
#define CC1101_PARTNUM      0x30
#define CC1101_VERSION      0x31
#define CC1101_FREQEST      0x32
#define CC1101_LQI          0x33
#define CC1101_RSSI         0x34
#define CC1101_MARCSTATE    0x35
#define CC1101_WORTIME1     0x36
#define CC1101_WORTIME0     0x37
#define CC1101_PKTSTATUS    0x38
#define CC1101_VCO_VC_DAC   0x39
#define CC1101_TXBYTES      0x3A
#define CC1101_RXBYTES      0x3B

//CC1101 PATABLE,TXFIFO,RXFIFO
#define CC1101_PATABLE      0x3E
#define CC1101_TXFIFO       0x3F
#define CC1101_RXFIFO       0x3F

//************************************* class **************************************************//
class SmartRC_CC1101
{
private:
  byte modulation = 2;
  byte chan = 0;
  int pa = 12;
  byte last_pa;
  byte SCK_PIN;
  byte MISO_PIN;
  byte MOSI_PIN;
  byte SS_PIN;
  byte GDO0;
  byte GDO2;
  bool spi_initialized = false;
  bool custom_spi_pins = false;
  bool ccmode = false;
  float MHz = 433.92;
  byte trxstate = 0;
  byte clb1[2] = {24, 28};
  byte clb2[2] = {31, 38};
  byte clb3[2] = {65, 76};
  byte clb4[2] = {77, 79};

  void SpiStart(void);
  void SpiEnd(void);
  bool WaitMiso(uint16_t timeout_ms = 200);
  void GDO_Set(void);
  void GDO0_Set(void);
  void Reset(void);
  void setSpi(void);
  void RegConfigSettings(void);
  void Calibrate(void);

public:
  void Init(void);
  byte SpiReadStatus(byte addr);
  void setSpiPin(byte sck, byte miso, byte mosi, byte ss);
  void setGDO(byte gdo0, byte gdo2);
  void setGDO0(byte gdo0);
  void setCCMode(bool s);
  void setModulation(byte m);
  void setPA(int p);
  
  void setMHZ(float mhz);
  void setChannel(byte chnl);
  void setChsp(float f);
  void setRxBW(float f);
  void setDRate(float d);
  void setDeviation(float d);
  
  bool getCC1101(void);
  float getMHZ(void);
  float getDRate(void);
  float getRxBW(void);
  float getChsp(void);

  void SetTx(void);
  void SetRx(void);
  void SetTx(float mhz);
  void SetRx(float mhz);
  int getRssi(void);
  byte getLqi(void);
  void setSres(void);
  void setSidle(void);
  void goSleep(void);
  
  void SendData(byte *txBuffer, byte size);
  void SendData(char *txchar);
  void SendData(byte *txBuffer, byte size, int t);
  void SendData(char *txchar, int t);
  
  byte CheckReceiveFlag(void);
  byte ReceiveData(byte *rxBuffer);
  bool CheckCRC(void);
  bool CheckRxFifo(int t);
  
  void SpiStrobe(byte strobe);
  void SpiWriteReg(byte addr, byte value);
  void SpiWriteBurstReg(byte addr, byte *buffer, byte num);
  byte SpiReadReg(byte addr);
  void SpiReadBurstReg(byte addr, byte *buffer, byte num);
  
  void setClb(byte b, byte s, byte e);
  byte getMode(void);
  
  void setSyncWord(byte sh, byte sl);
  void setAddr(byte v);
  void setWhiteData(bool v);
  void setPktFormat(byte v);
  void setCrc(bool v);
  void setLengthConfig(byte v);
  void setPacketLength(byte v);
  void setDcFilterOff(bool v);
  void setManchester(bool v);
  void setSyncMode(byte v);
  void setFEC(bool v);
  void setPRE(byte v);
  void setPQT(byte v);
  void setCRC_AF(bool v);
  void setAppendStatus(bool v);
  void setAdrChk(byte v);
};

// ==============================================================================
// LEGACY SUPPORT / BACKWARD COMPATIBILITY
// These lines ensure that old scripts continue to run without changes!
// ==============================================================================

// Maps the old class name to the new one
typedef SmartRC_CC1101 ELECHOUSE_CC1101;

// Provides the standard, global objects
extern SmartRC_CC1101& ELECHOUSE_cc1101; // Old name
extern SmartRC_CC1101 SmartRC_cc1101;   // New name

#endif