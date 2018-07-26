/*
  Example for outlets which are configured with two rotary/sliding switches.
  
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

#define esp 0 // for ESP8266 set 1, for Arduino set 0.

int pin; // int for Transmit pin.

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  
  if (esp == 1){pin = 4;} // for esp8266! Transmit on pin 4 = D2.
  if (esp == 0){pin = 2;} // for Arduino! Transmit on pin 2

//CC1101 Settings:                (Settings with "//" are optional!)
ELECHOUSE_cc1101.setESP8266(esp); // esp8266 & Arduino SPI pin settings. Don´t change this line!
//ELECHOUSE_cc1101.setRxBW(16); // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(175);// set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
//ELECHOUSE_cc1101.setChsp(80); // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
//ELECHOUSE_cc1101.freq2(0x10); //set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq1(0x27); //set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq0(0x62); //set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.
  ELECHOUSE_cc1101.Init(F_433); //Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx. set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30. example: F_433_PA10. (default = PA10)
 
  // Transmitter on 
  mySwitch.enableTransmit(pin);

  // cc1101 set Transmit on
  ELECHOUSE_cc1101.SetTx()
  
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the setting of the first rotary switch. 
  // In this example it's switched to "1" or "A" or "I". 
  // 
  // The second parameter represents the setting of the second rotary switch. 
  // In this example it's switched to "4" or "D" or "IV". 
  mySwitch.switchOn(1, 4);

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff(1, 4);
  
  // Wait another second
  delay(1000);
  
}
