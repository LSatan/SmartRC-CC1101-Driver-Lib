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
#include <SPI.h>
#include "ELECHOUSE_CC1101_RCS_DRV.h"
#include <Arduino.h>

/****************************************************************/
#define   WRITE_BURST       0x40            //write burst
#define   READ_SINGLE       0x80            //read single
#define   READ_BURST        0xC0            //read burst
#define   BYTES_IN_RXFIFO   0x47            //byte number in RXfifo

int mdcf1 = 0x00;
int mdcf0 = 0xF8;
int chan = 1;
int rbw = 0x07;
int F2 = 16;
int F1 = 176;
int F0 = 113;

int conf = PA10;

int SCK_PIN = 13;
int MISO_PIN = 12;
int MOSI_PIN = 11;
int SS_PIN = 10;
int GDO0;
int GDO2;
int spi;

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
*FUNCTION NAME:SpiStart
*FUNCTION     :spi communication start
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiStart(void)
{
  // initialize the SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);

  // enable SPI
  #ifdef ESP32
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  #else
  SPI.begin();
  #endif
}
/****************************************************************
*FUNCTION NAME:SpiEnd
*FUNCTION     :spi communication disable
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiEnd(void)
{
  // disable SPI
  SPI.endTransaction();
  SPI.end();
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
	pinMode(GDO2, OUTPUT);
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
  SPI.transfer(CC1101_SRES);
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
  setSpi();
	SpiStart();										//spi initialization
	GDO_Set();										//GDO set
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);
	Reset();										//CC1101 reset
	RegConfigSettings(conf);						//CC1101 register config
  SpiEnd();
}
/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::Init(byte f)
{
  setSpi();  
  conf = (f);
	SpiStart();										//spi initialization
	GDO_Set();										//GDO set
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);
	Reset();										//CC1101 reset
	RegConfigSettings(conf);							//CC1101 register config
  SpiEnd();
}
/****************************************************************
*FUNCTION NAME:SpiWriteReg
*FUNCTION     :CC1101 write data to register
*INPUT        :addr: register address; value: register value
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiWriteReg(byte addr, byte value)
{
  SpiStart();
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
  SPI.transfer(addr);
  SPI.transfer(value); 
	digitalWrite(SS_PIN, HIGH);
  SpiEnd();
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
  SpiStart();
	temp = addr | WRITE_BURST;
  digitalWrite(SS_PIN, LOW);
  while(digitalRead(MISO_PIN));
  SPI.transfer(temp);
  for (i = 0; i < num; i++)
 	{
  SPI.transfer(buffer[i]);
  }
  digitalWrite(SS_PIN, HIGH);
  SpiEnd();
}
/****************************************************************
*FUNCTION NAME:SpiStrobe
*FUNCTION     :CC1101 Strobe
*INPUT        :strobe: command; //refer define in CC1101.h//
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SpiStrobe(byte strobe)
{
  SpiStart();
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
  SPI.transfer(strobe);
	digitalWrite(SS_PIN, HIGH);
  SpiEnd();
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
  SpiStart();
  temp = addr| READ_SINGLE;
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
  SPI.transfer(temp);
  value=SPI.transfer(0);
	digitalWrite(SS_PIN, HIGH);
  SpiEnd();
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
  SpiStart();
	temp = addr | READ_BURST;
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
  SPI.transfer(temp);
	for(i=0;i<num;i++)
	{
  buffer[i]=SPI.transfer(0);
	}
	digitalWrite(SS_PIN, HIGH);
  SpiEnd();
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
  SpiStart();
	temp = addr | READ_BURST;
	digitalWrite(SS_PIN, LOW);
	while(digitalRead(MISO_PIN));
  SPI.transfer(temp);
  value=SPI.transfer(0);
	digitalWrite(SS_PIN, HIGH);
  SpiEnd();
	return value;
}
/****************************************************************
*FUNCTION NAME:SPI pin Settings
*FUNCTION     :Set Spi pins
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setSpi(void){
  if (spi == 1){}else{
  #if defined __AVR_ATmega168__ || defined __AVR_ATmega328P__
  SCK_PIN = 13; MISO_PIN = 12; MOSI_PIN = 11; SS_PIN = 10;
  #elif defined __AVR_ATmega1280__ || defined __AVR_ATmega2560__
  SCK_PIN = 52; MISO_PIN = 50; MOSI_PIN = 51; SS_PIN = 53;
  #elif ESP8266
  SCK_PIN = 14; MISO_PIN = 12; MOSI_PIN = 13; SS_PIN = 15;
  #elif ESP32
  SCK_PIN = 18; MISO_PIN = 19; MOSI_PIN = 23; SS_PIN = 5;
  #else
  SCK_PIN = 13; MISO_PIN = 12; MOSI_PIN = 11; SS_PIN = 10;
  #endif
}
}
/****************************************************************
*FUNCTION NAME:COSTUM SPI
*FUNCTION     :set costum spi pins.
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setSpiPin(byte sck, byte miso, byte mosi, byte ss){
  spi = 1;
  SCK_PIN = sck;
  MISO_PIN = miso;
  MOSI_PIN = mosi;
  SS_PIN = ss;
}
/****************************************************************
*FUNCTION NAME:Frequency Calculator
*FUNCTION     :Calculate the basic frequency.
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setMHZ(float mhz){

float MHZ = mhz + 0.01;

float freq2 = 26;
float freq1 = 0.1015625;
float freq0 = 0.00039675;

float s1 = MHZ/freq2; 
int s2 = s1;                             // freq2
float s3 = s1-s2;
float s4 = s3*100000000;
float s5 = 255.0/100000000*s4;
int s6 = s5;
float s7 = (s5-s6);
float s8 = s7*10;
int s9;                                  // freq1
if (s8>=5){s9=s6+1;}
if (s8<5){s9=s6;}
float s10 = MHZ-(freq2*s2+freq1*s9);
float s11 = s10/freq0;
int s12 = s11;
float s13 = (s11-s12)*(10);
int s14;                                 //freq0
if (s13>=5){s14=s12+1;}
if (s13<5){s14=s12;}

F2 = s2;
F1 = s9;
F0 = s14;
}
/****************************************************************
*FUNCTION NAME:GDO Pin settings
*FUNCTION     :set GDO Pins
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setGDO(byte gdo0, byte gdo2){
GDO0 = gdo0;
GDO2 = gdo2;  
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
case 1: mdcf1=0x00; mdcf0=0x00; break; case 2: mdcf1=0x00; mdcf0=0xF8; break; case 3: mdcf1=0x01; mdcf0=0x93; break; case 4: mdcf1=0x01; mdcf0=0xF8; break; case 5: mdcf1=0x02; mdcf0=0x7A; break; 
case 6: mdcf1=0x02; mdcf0=0xF8; break; case 7: mdcf1=0x03; mdcf0=0x3B; break; case 8: mdcf1=0x03; mdcf0=0x7A; break; case 9: mdcf1=0x03; mdcf0=0xB9; break; case 10: mdcf1=0x03; mdcf0=0xFF; break;  
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
chan = chnl;
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
    SpiWriteReg(CC1101_FREQ2,    F2);
    SpiWriteReg(CC1101_FREQ1,    F1);
    SpiWriteReg(CC1101_FREQ0,    F0);
      
    switch(f)
    {
    case PA10:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE10,8);
        break;
    case PA7:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE7,8);
        break;
    case PA5:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE5,8);
        break;        
    case PA0:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE0,8);
        break;
    case PA_10:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_10,8);
        break;
    case PA_15:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_15,8);
        break;
    case PA_20:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_20,8);
        break;
    case PA_30:
        SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE_30,8);
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
  SpiStrobe(CC1101_SRES);
  SpiStart();                    //spi initialization
  GDO_Set();                    //GDO set
  digitalWrite(SS_PIN, HIGH);
  digitalWrite(SCK_PIN, HIGH);
  digitalWrite(MOSI_PIN, LOW);
  Reset();                      //CC1101 reset
  RegConfigSettings(conf);      //CC1101 register config
  SpiStrobe(CC1101_STX);        //start send
}
/****************************************************************
*FUNCTION NAME:SetRx
*FUNCTION     :set CC1101 to receive state
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SetRx(void)
{
  SpiStrobe(CC1101_SRES);
  SpiStart();                    //spi initialization
  GDO_Set();                    //GDO set
  digitalWrite(SS_PIN, HIGH);
  digitalWrite(SCK_PIN, HIGH);
  digitalWrite(MOSI_PIN, LOW);
  Reset();                      //CC1101 reset
  RegConfigSettings(conf);      //CC1101 register config
  SpiStrobe(CC1101_SRX);        //start receive
}
/****************************************************************
*FUNCTION NAME:SetTx
*FUNCTION     :set CC1101 send data
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SetTx(float mhz)
{
  SpiStrobe(CC1101_SRES);
  setMHZ(mhz);
  SpiStart();                    //spi initialization
  GDO_Set();                    //GDO set
  digitalWrite(SS_PIN, HIGH);
  digitalWrite(SCK_PIN, HIGH);
  digitalWrite(MOSI_PIN, LOW);
  Reset();                      //CC1101 reset
  RegConfigSettings(conf);      //CC1101 register config
  SpiStrobe(CC1101_STX);        //start send
}
/****************************************************************
*FUNCTION NAME:SetRx
*FUNCTION     :set CC1101 to receive state
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::SetRx(float mhz)
{
  SpiStrobe(CC1101_SRES);
  setMHZ(mhz);
  SpiStart();                    //spi initialization
  GDO_Set();                    //GDO set
  digitalWrite(SS_PIN, HIGH);
  digitalWrite(SCK_PIN, HIGH);
  digitalWrite(MOSI_PIN, LOW);
  Reset();                      //CC1101 reset
  RegConfigSettings(conf);      //CC1101 register config
  SpiStrobe(CC1101_SRX);        //start receive
}
/****************************************************************
*FUNCTION NAME:SetSres
*FUNCTION     :Reset CC1101
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void ELECHOUSE_CC1101::setSres(void)
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




