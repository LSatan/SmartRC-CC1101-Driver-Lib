/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
//CC1101 Settings:  
//ELECHOUSE_cc1101.setRxBW(1); // set Receive filter bandwidth (default = 58khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(175);// set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
//ELECHOUSE_cc1101.setChsp(80); // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
//ELECHOUSE_cc1101.freq2(0x10); //set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq1(0x27); //set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq0(0x62); //set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.
  ELECHOUSE_cc1101.Init(F_433); //Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx. set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30. example: F_433_PA10. (default = PA0)
 
  // Transmitter is connected to Arduino Pin #2  
  mySwitch.enableTransmit(2);

  // cc1101 set Transmit on
  ELECHOUSE_cc1101.SetTx();
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* See Example: TypeA_WithDIPSwitches */
  mySwitch.switchOn("11111", "00010");
  delay(1000);
  mySwitch.switchOff("11111", "00010");
  delay(1000);

  /* Same switch as above, but using decimal code */
  mySwitch.send(5393, 24);
  delay(1000);  
  mySwitch.send(5396, 24);
  delay(1000);  

  /* Same switch as above, but using binary code */
  mySwitch.send("000000000001010100010001");
  delay(1000);  
  mySwitch.send("000000000001010100010100");
  delay(1000);

  /* Same switch as above, but tri-state code */ 
  mySwitch.sendTriState("00000FFF0F0F");
  delay(1000);  
  mySwitch.sendTriState("00000FFF0FF0");
  delay(1000);

  delay(20000);
}
