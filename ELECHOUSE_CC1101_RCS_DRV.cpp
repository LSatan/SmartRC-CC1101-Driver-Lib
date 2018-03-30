/*
  ELECHOUSE_CC1101.cpp - CC1101 module library
  Copyright (c) 2010 Michael.
    Author: Michael, <www.elechouse.com>
    Version: November 12, 2010

  This library is designed to use CC1101/CC1100 module on Arduino platform.
  CC1101/CC1100 module is an useful wireless module.Using the functions of the 
  library, you can easily send and receive data by the CC1101/CC1100 module. 
  Just have fun!
  For the details, please refer to the datasheet of CC1100/CC1101.
----------------------------------------------------------------------------------------------------------------
cc1101 Driver for RC Switch. Mod by Little Satan. With permission to modify and publish Wilson Shen (ELECHOUSE).
----------------------------------------------------------------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <Arduino.h>

/****************************************************************/
#define 	WRITE_BURST     	0x40						//write burst
#define 	READ_SINGLE     	0x80						//read single
#define 	READ_BURST      	0xC0						//read burst
#define 	BYTES_IN_RXFIFO     0x47  						//byte number in RXfifo

int mdcf1 = 0x01;
int mdcf0 = 0x93;
int chan = 0xAF;
int rbw = 0xF7;
int F2;
int F1;
int F0;
/****************************************************************/
uint8_t PA_TABLE10[8] {0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE7[8] {0x00,0xC8,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE5[8] {0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE0[8] {0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE_10[8] {0x00,0x34,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE_15[8] {0x00,0x1D,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE_20[8] {0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,};
uint8_t PA_TABLE_30[8] {0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,};

/****************************************************************
*FUNCTION NAME:SpiInit
*FUNCTION     :spi communication initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiInit(void)
{
  // initialize the SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);

  // enable SPI Master, MSB, SPI mode 0, FOSC/4
  SpiMode(0);
}
/****************************************************************
*FUNCTION NAME:SpiMode
*FUNCTION     :set spi mode
*INPUT        :        config               mode
			   (0<<CPOL) | (0 << CPHA)		 0
			   (0<<CPOL) | (1 << CPHA)		 1
			   (1<<CPOL) | (0 << CPHA)		 2
			   (1<<CPOL) | (1 << CPHA)		 3
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiMode(byte config)
{
  byte tmp;

  // enable SPI master with configuration byte specified
  SPCR = 0;
  SPCR = (config & 0x7F) | (1<<SPE) | (1<<MSTR);
  tmp = SPSR;
  tmp = SPDR;
}

/****************************************************************
*FUNCTION NAME:SpiTransfer
*FUNCTION     :spi transfer
*INPUT        :value: data to send
*OUTPUT       :data to receive
****************************************************************/
byte ELECHOUSE_CC1101::SpiTransfer(byte value)
{
  SPDR = value;
  while (!(SPSR & (1<<SPIF))) ;
  return SPDR;
}

/****************************************************************
*FUNCTION NAME: GDO_Set()
*FUNCTION     : set GDO0,GDO2 pin
*INPUT        : none
*OUTPUT       : none
****************************************************************/
void ELECHOUSE_CC1101::GDO_Set (void)
{
	pinMode(GDO0, INPUT);
	pinMode(GDO2, INPUT);
}

/****************************************************************
*FUNCTION NAME:Reset
*FUNCTION     :CC1101 reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::Reset (void)
{
	digitalWrite(SS_PIN, LOW);
	delay(1);
	digitalWrite(SS_PIN, HIGH);
	delay(1);
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
	SpiTransfer(CC1101_SRES);
	while(digitalRead(MISO_PIN));
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::Init(void)
{
	SpiInit();										//spi initialization
	GDO_Set();										//GDO set
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);
	Reset();										//CC1101 reset
	RegConfigSettings(F_433);						//CC1101 register config
	//SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::Init(byte f)
{
	SpiInit();										//spi initialization
	GDO_Set();										//GDO set
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);
	Reset();										//CC1101 reset
	RegConfigSettings(f);							//CC1101 register config
	//SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}

/****************************************************************
*FUNCTION NAME:SpiWriteReg
*FUNCTION     :CC1101 write data to register
*INPUT        :addr: register address; value: register value
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiWriteReg(byte addr, byte value)
{
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
	SpiTransfer(addr);
	SpiTransfer(value);
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiWriteBurstReg
*FUNCTION     :CC1101 write burst data to register
*INPUT        :addr: register address; buffer:register value array; num:number to write
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiWriteBurstReg(byte addr, byte *buffer, byte num)
{
	byte i, temp;

	temp = addr | WRITE_BURST;
    digitalWrite(SS_PIN, LOW);
    while(digitalRead(MISO_PIN));
    SpiTransfer(temp);
    for (i = 0; i < num; i++)
 	{
        SpiTransfer(buffer[i]);
    }
    digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiStrobe
*FUNCTION     :CC1101 Strobe
*INPUT        :strobe: command; //refer define in CC1101.h//
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiStrobe(byte strobe)
{
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
	SpiTransfer(strobe);
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiReadReg
*FUNCTION     :CC1101 read data from register
*INPUT        :addr: register address
*OUTPUT       :register value
****************************************************************/
byte ELECHOUSE_CC1101::SpiReadReg(byte addr) 
{
	byte temp, value;

    temp = addr|READ_SINGLE;
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
	SpiTransfer(temp);
	value=SpiTransfer(0);
	digitalWrite(SS_PIN, HIGH);

	return value;
}

/****************************************************************
*FUNCTION NAME:SpiReadBurstReg
*FUNCTION     :CC1101 read burst data from register
*INPUT        :addr: register address; buffer:array to store register value; num: number to read
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiReadBurstReg(byte addr, byte *buffer, byte num)
{
	byte i,temp;

	temp = addr | READ_BURST;
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
	SpiTransfer(temp);
	for(i=0;i<num;i++)
	{
		buffer[i]=SpiTransfer(0);
	}
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiReadStatus
*FUNCTION     :CC1101 read status register
*INPUT        :addr: register address
*OUTPUT       :status value
****************************************************************/
byte ELECHOUSE_CC1101::SpiReadStatus(byte addr) 
{
	byte value,temp;

	temp = addr | READ_BURST;
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
	SpiTransfer(temp);
	value=SpiTransfer(0);
	digitalWrite(SS_PIN, HIGH);

	return value;
}
/****************************************************************
*FUNCTION NAME:Set Tx Power
*FUNCTION     :Tx Power dbm
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::freq2(byte F2_xxx){
F2 = F2_xxx;
}
void ELECHOUSE_CC1101::freq1(byte F1_xxx){
F1 = F1_xxx;
}
void ELECHOUSE_CC1101::freq0(byte F0_xxx){
F0 = F0_xxx;
}
/****************************************************************
*FUNCTION NAME:Channel spacing
*FUNCTION     :Channel spacing
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setChsp(byte Chsp){

switch (Chsp)
{
case 25: mdcf1=0x00; mdcf0=0x00; break; case 50: mdcf1=0x00; mdcf0=0xF8; break; case 80: mdcf1=0x01; mdcf0=0x93; break; case 100: mdcf1=0x01; mdcf0=0xF8; break; case 150: mdcf1=0x02; mdcf0=0x7A; break; 
case 200: mdcf1=0x02; mdcf0=0xF8; break; case 250: mdcf1=0x03; mdcf0=0x3B; break; case 300: mdcf1=0x03; mdcf0=0x7A; break; case 350: mdcf1=0x03; mdcf0=0xB9; break; case 405: mdcf1=0x03; mdcf0=0xFF; break;  
}
}
/****************************************************************
*FUNCTION NAME:Set RX bandwidth
*FUNCTION     :Recive bandwidth
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setRxBW(byte RxBW){

switch (RxBW)
{
case 1: rbw=0xF7; break; case 2: rbw=0xE7; break; case 3: rbw=0xD7; break; case 4: rbw=0xC7; break; case 5: rbw=0xB7; break; case 6: rbw=0xA7; break; case 7: rbw=0x97; break; case 8: rbw=0x87; break;
case 9: rbw=0x77; break; case 10: rbw=0x67; break; case 11: rbw=0x57; break; case 12: rbw=0x47; break; case 13: rbw=0x37; break; case 14: rbw=0x27; break; case 15: rbw=0x17; break; case 16: rbw=0x07; break; 
}
}
/****************************************************************
*FUNCTION NAME:Set Channel
*FUNCTION     :Recive channel from sketch 
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setChannel(byte chnl){

switch (chnl)
{
case 0: chan=0x00; break; case 1: chan=0x01; break; case 2: chan=0x02; break; case 3: chan=0x03; break; case 4: chan=0x04; break; case 5: chan=0x05; break; case 6: chan=0x06; break; case 7: chan=0x07; break; case 8: chan=0x08; break; 
case 9: chan=0x09; break; case 10: chan=0x0A; break; case 11: chan=0x0B; break; case 12: chan=0x0C; break; case 13: chan=0x0D; break; case 14: chan=0x0E; break; case 15: chan=0x0F; break; case 16: chan=0x10; break; case 17: chan=0x11; break; 
case 18: chan=0x12; break; case 19: chan=0x13; break; case 20: chan=0x14; break; case 21: chan=0x15; break; case 22: chan=0x16; break; case 23: chan=0x17; break; case 24: chan=0x18; break; case 25: chan=0x19; break; case 26: chan=0x1A; break; 
case 27: chan=0x1B; break; case 28: chan=0x1C; break; case 29: chan=0x1D; break; case 30: chan=0x1E; break; case 31: chan=0x1F; break; case 32: chan=0x20; break; case 33: chan=0x21; break; case 34: chan=0x22; break; case 35: chan=0x23; break; 
case 36: chan=0x24; break; case 37: chan=0x25; break; case 38: chan=0x26; break; case 39: chan=0x27; break; case 40: chan=0x28; break; case 41: chan=0x29; break; case 42: chan=0x2A; break; case 43: chan=0x2B; break; case 44: chan=0x2C; break; 
case 45: chan=0x2D; break; case 46: chan=0x2E; break; case 47: chan=0x2F; break; case 48: chan=0x30; break; case 49: chan=0x31; break; case 50: chan=0x32; break; case 51: chan=0x33; break; case 52: chan=0x34; break; case 53: chan=0x35; break; 
case 54: chan=0x36; break; case 55: chan=0x37; break; case 56: chan=0x38; break; case 57: chan=0x39; break; case 58: chan=0x3A; break; case 59: chan=0x3B; break; case 60: chan=0x3C; break; case 61: chan=0x3D; break; case 62: chan=0x3E; break; 
case 63: chan=0x3F; break; case 64: chan=0x40; break; case 65: chan=0x41; break; case 66: chan=0x42; break; case 67: chan=0x43; break; case 68: chan=0x44; break; case 69: chan=0x45; break; case 70: chan=0x46; break; case 71: chan=0x47; break; 
case 72: chan=0x48; break; case 73: chan=0x49; break; case 74: chan=0x4A; break; case 75: chan=0x4B; break; case 76: chan=0x4C; break; case 77: chan=0x4D; break; case 78: chan=0x4E; break; case 79: chan=0x4F; break; case 80: chan=0x50; break;
case 81: chan=0x51; break; case 82: chan=0x52; break; case 83: chan=0x53; break; case 84: chan=0x54; break; case 85: chan=0x55; break; case 86: chan=0x56; break; case 87: chan=0x57; break; case 88: chan=0x58; break; case 89: chan=0x59; break; 
case 90: chan=0x5A; break; case 91: chan=0x5B; break; case 92: chan=0x5C; break; case 93: chan=0x5D; break; case 94: chan=0x5E; break; case 95: chan=0x5F; break; case 96: chan=0x60; break; case 97: chan=0x61; break; case 98: chan=0x62; break; 
case 99: chan=0x63; break; case 100: chan=0x64; break; case 101: chan=0x65; break; case 102: chan=0x66; break; case 103: chan=0x67; break; case 104: chan=0x68; break; case 105: chan=0x69; break; case 106: chan=0x6A; break; case 107: chan=0x6B; break;
case 108: chan=0x6C; break; case 109: chan=0x6D; break; case 110: chan=0x6E; break; case 111: chan=0x6F; break; case 112: chan=0x70; break; case 113: chan=0x71; break; case 114: chan=0x72; break; case 115: chan=0x73; break; case 116: chan=0x74; break; 
case 117: chan=0x75; break; case 118: chan=0x76; break; case 119: chan=0x77; break; case 120: chan=0x78; break; case 121: chan=0x79; break; case 122: chan=0x7A; break; case 123: chan=0x7B; break; case 124: chan=0x7C; break; case 125: chan=0x7D; break; 
case 126: chan=0x7E; break; case 127: chan=0x7F; break; case 128: chan=0x80; break; case 129: chan=0x81; break; case 130: chan=0x82; break; case 131: chan=0x83; break; case 132: chan=0x84; break; case 133: chan=0x85; break; case 134: chan=0x86; break;
case 135: chan=0x87; break; case 136: chan=0x88; break; case 137: chan=0x89; break; case 138: chan=0x8A; break; case 139: chan=0x8B; break; case 140: chan=0x8C; break; case 141: chan=0x8D; break; case 142: chan=0x8E; break; case 143: chan=0x8F; break; 
case 144: chan=0x90; break; case 145: chan=0x91; break; case 146: chan=0x92; break; case 147: chan=0x93; break; case 148: chan=0x94; break; case 149: chan=0x95; break; case 150: chan=0x96; break; case 151: chan=0x97; break; case 152: chan=0x98; break; 
case 153: chan=0x99; break; case 154: chan=0x9A; break; case 155: chan=0x9B; break; case 156: chan=0x9C; break; case 157: chan=0x9D; break; case 158: chan=0x9E; break; case 159: chan=0x9F; break; case 160: chan=0xA0; break; case 161: chan=0xA1; break;
case 162: chan=0xA2; break; case 163: chan=0xA3; break; case 164: chan=0xA4; break; case 165: chan=0xA5; break; case 166: chan=0xA6; break; case 167: chan=0xA7; break; case 168: chan=0xA8; break; case 169: chan=0xA9; break; case 170: chan=0xAA; break; 
case 171: chan=0xAB; break; case 172: chan=0xAC; break; case 173: chan=0xAD; break; case 174: chan=0xAE; break; case 175: chan=0xAF; break; case 176: chan=0xB0; break; case 177: chan=0xB1; break; case 178: chan=0xB2; break; case 179: chan=0xB3; break; 
case 180: chan=0xB4; break; case 181: chan=0xB5; break; case 182: chan=0xB6; break; case 183: chan=0xB7; break; case 184: chan=0xB8; break; case 185: chan=0xB9; break; case 186: chan=0xBA; break; case 187: chan=0xBB; break; case 188: chan=0xBC; break;
case 189: chan=0xBD; break; case 190: chan=0xBE; break; case 191: chan=0xBF; break; case 192: chan=0xC0; break; case 193: chan=0xC1; break; case 194: chan=0xC2; break; case 195: chan=0xC3; break; case 196: chan=0xC4; break; case 197: chan=0xC5; break; 
case 198: chan=0xC6; break; case 199: chan=0xC7; break; case 200: chan=0xC8; break; case 201: chan=0xC9; break; case 202: chan=0xCA; break; case 203: chan=0xCB; break; case 204: chan=0xCC; break; case 205: chan=0xCD; break; case 206: chan=0xCE; break; 
case 207: chan=0xCF; break; case 208: chan=0xD0; break; case 209: chan=0xD1; break; case 210: chan=0xD2; break; case 211: chan=0xD3; break; case 212: chan=0xD4; break; case 213: chan=0xD5; break; case 214: chan=0xD6; break; case 215: chan=0xD7; break;
case 216: chan=0xD8; break; case 217: chan=0xD9; break; case 218: chan=0xDA; break; case 219: chan=0xDB; break; case 220: chan=0xDC; break; case 221: chan=0xDD; break; case 222: chan=0xDE; break; case 223: chan=0xDF; break; case 224: chan=0xE0; break; 
case 225: chan=0xE1; break; case 226: chan=0xE2; break; case 227: chan=0xE3; break; case 228: chan=0xE4; break; case 229: chan=0xE5; break; case 230: chan=0xE6; break; case 231: chan=0xE7; break; case 232: chan=0xE8; break; case 233: chan=0xE9; break; 
case 234: chan=0xEA; break; case 235: chan=0xEB; break; case 236: chan=0xEC; break; case 237: chan=0xED; break; case 238: chan=0xEE; break; case 239: chan=0xEF; break; case 240: chan=0xF0; break; case 241: chan=0xF1; break; case 242: chan=0xF2; break;
case 243: chan=0xF3; break; case 244: chan=0xF4; break; case 245: chan=0xF5; break; case 246: chan=0xF6; break; case 247: chan=0xF7; break; case 248: chan=0xF8; break; case 249: chan=0xF9; break; case 250: chan=0xFA; break; case 251: chan=0xFB; break; 
case 252: chan=0xFC; break; case 253: chan=0xFD; break; case 254: chan=0xFE; break; case 255: chan=0xFF; break;
}
}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :CC1101 register config //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::RegConfigSettings(byte f) 
{

    SpiWriteReg(CC1101_FSCTRL1,  0x06);
    SpiWriteReg(CC1101_FSCTRL0,  0x00);
	    
    switch(f)
    {
      case F_868:
      	SpiWriteReg(CC1101_FREQ2,    F2_868);
      	SpiWriteReg(CC1101_FREQ1,    F1_868);
      	SpiWriteReg(CC1101_FREQ0,    F0_868);
      	SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
      case F_868_PA10:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE10,8);
        break;
      case F_868_PA7:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE7,8);
        break;
      case F_868_PA5:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE5,8);
        break;
      case F_868_PA0:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
      case F_868_PA_10:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_10,8);
        break;
      case F_868_PA_15:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_15,8);
        break;
      case F_868_PA_20:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_20,8);
        break;   
      case F_868_PA_30:
        SpiWriteReg(CC1101_FREQ2,    F2_868);
        SpiWriteReg(CC1101_FREQ1,    F1_868);
        SpiWriteReg(CC1101_FREQ0,    F0_868);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_30,8);
        break;          

      case F_915:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
      case F_915_PA10:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE10,8);
        break;
      case F_915_PA7:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE7,8);
        break;
      case F_915_PA5:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE5,8);
        break;
      case F_915_PA0:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
      case F_915_PA_10:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_10,8);
        break;
      case F_915_PA_15:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_15,8);
        break;
      case F_915_PA_20:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_20,8);
        break;        
      case F_915_PA_30:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_30,8);
        break;
        
	  case F_433:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case F_433_PA10:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE10,8);
        break;
    case F_433_PA7:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE7,8);
        break;
    case F_433_PA5:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE5,8);
        break;
    case F_433_PA0:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case F_433_PA_10:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_10,8);
        break;  
    case F_433_PA_15:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_15,8);
        break;
    case F_433_PA_20:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_20,8);
        break;                      
    case F_433_PA_30:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_30,8);
        break;
        
    case F_315:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case F_315_PA10:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE10,8);
        break;
    case F_315_PA7:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE7,8);
        break;
    case F_315_PA5:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE5,8);
        break;
    case F_315_PA0:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case F_315_PA_10:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_10,8);
        break;
    case F_315_PA_15:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_15,8);
        break;
    case F_315_PA_20:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_20,8);
        break;
    case F_315_PA_30:
        SpiWriteReg(CC1101_FREQ2,    F2_315);
        SpiWriteReg(CC1101_FREQ1,    F1_315);
        SpiWriteReg(CC1101_FREQ0,    F0_315);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_30,8);
        break;

    case F_xxx:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case F_xxx_PA10:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE10,8);
        break;
    case F_xxx_PA7:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE7,8);
        break;
    case F_xxx_PA5:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE5,8);
        break;        
    case F_xxx_PA0:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case F_xxx_PA_10:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_10,8);
        break;
    case F_xxx_PA_15:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_15,8);
        break;
    case F_xxx_PA_20:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_20,8);
        break;
    case F_xxx_PA_30:
        SpiWriteReg(CC1101_FREQ2,    F2);
        SpiWriteReg(CC1101_FREQ1,    F1);
        SpiWriteReg(CC1101_FREQ0,    F0);
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_30,8);
        break;

	  default: // F must be set
	  	break;
	}
	
    SpiWriteReg(CC1101_MDMCFG4,  rbw);
    SpiWriteReg(CC1101_MDMCFG3,  0x93);
    SpiWriteReg(CC1101_MDMCFG2,  0x30);
    SpiWriteReg(CC1101_MDMCFG1,  mdcf1);
    SpiWriteReg(CC1101_MDMCFG0,  mdcf0);
    SpiWriteReg(CC1101_CHANNR,   chan);
    SpiWriteReg(CC1101_DEVIATN,  0x15);
    SpiWriteReg(CC1101_FREND1,   0x56);
    SpiWriteReg(CC1101_FREND0,   0x11);
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
    SpiWriteReg(CC1101_IOCFG2,   0x0D); 	//serial clock.synchronous to the data in synchronous serial mode
    SpiWriteReg(CC1101_IOCFG0,   0x0D);  	//asserts when sync word has been sent/received, and de-asserts at the end of the packet 
    SpiWriteReg(CC1101_PKTCTRL1, 0x04);		//two status bytes will be appended to the payload of the packet,including RSSI LQI and CRC OK
											//No address check
    SpiWriteReg(CC1101_PKTCTRL0, 0x32);		//whitening off;CRC Enable��variable length packets, packet length configured by the first byte after sync word
    SpiWriteReg(CC1101_ADDR,     0x00);		//address used for packet filtration.
    SpiWriteReg(CC1101_PKTLEN,   0x3D); 	//61 bytes max length
}

/****************************************************************
*FUNCTION NAME:SetTx
*FUNCTION     :set CC1101 send data
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SetTx(void)
{
	SpiStrobe(CC1101_STX);									//start send	
}

/****************************************************************
*FUNCTION NAME:SetRx
*FUNCTION     :set CC1101 to receive state
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SetRx(void)
{
	SpiStrobe(CC1101_SRX);                  //start recive
}

/****************************************************************
*FUNCTION NAME:SetSres
*FUNCTION     :Reset CC1101
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SetSres(void)
{
  SpiStrobe(CC1101_SRES);                  //reset cc1101  
}

/****************************************************************
*FUNCTION NAME:CheckReceiveFlag
*FUNCTION     :check receive data or not
*INPUT        :none
*OUTPUT       :flag: 0 no data; 1 receive data 
****************************************************************/
byte ELECHOUSE_CC1101::CheckReceiveFlag(void)
{
	if(digitalRead(GDO0))			//receive data
	{
		while (digitalRead(GDO0));
		return 1;
	}
	else							// no data
	{
		return 0;
	}
}


/****************************************************************
*FUNCTION NAME:ReceiveData
*FUNCTION     :read data received from RXfifo
*INPUT        :rxBuffer: buffer to store data
*OUTPUT       :size of data received
****************************************************************/
byte ELECHOUSE_CC1101::ReceiveData(byte *rxBuffer)
{
	byte size;
	byte status[2];

	if(SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO)
	{
		size=SpiReadReg(CC1101_RXFIFO);
		SpiReadBurstReg(CC1101_RXFIFO,rxBuffer,size);
		SpiReadBurstReg(CC1101_RXFIFO,status,2);
		SpiStrobe(CC1101_SFRX);
		return size;
	}
	else
	{
		SpiStrobe(CC1101_SFRX);
		return 0;
	}
	
}

ELECHOUSE_CC1101 ELECHOUSE_cc1101;




