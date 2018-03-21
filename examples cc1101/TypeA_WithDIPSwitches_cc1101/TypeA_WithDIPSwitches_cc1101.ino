/*
  Example for outlets which are configured with a 10 pole DIP switch.
  
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
 
  ELECHOUSE_cc1101.setChannel(0xAF);// set channel in hex 80khz steps. channel number is 175 = 0xAF = 433,92Mhz / 315Mhz 
  ELECHOUSE_cc1101.Init(F_433);    // set frequency - F_315, F_433, F_868, F_965 MHz
 
  // Transmitter is connected to Arduino Pin #2  
  mySwitch.enableTransmit(2);

  // cc1101 set Transmit on
  ELECHOUSE_cc1101.SetTx();


  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the setting of the first 5 DIP switches. 
  // In this example it's ON-ON-OFF-OFF-ON.
  // 
  // The second parameter represents the setting of the last 5 DIP switches. 
  // In this example the last 5 DIP switches are OFF-ON-OFF-ON-OFF.  
  mySwitch.switchOn("11001", "01010");

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff("11001", "01010");
  
  // Wait another second
  delay(1000);
  
}
