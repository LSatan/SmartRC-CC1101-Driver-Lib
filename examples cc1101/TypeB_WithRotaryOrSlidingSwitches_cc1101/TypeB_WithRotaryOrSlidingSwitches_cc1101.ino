/*
  Example for outlets which are configured with two rotary/sliding switches.
  
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  // set frequency - F_433, F_868, F_965 MHz
  ELECHOUSE_cc1101.Init(F_433);
 
  // Transmitter is connected to Arduino Pin #2  
  mySwitch.enableTransmit(2);

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
