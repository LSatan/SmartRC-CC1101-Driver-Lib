/*
  Example for REV outlets (e.g. 8342L)
  
  https://github.com/sui77/rc-switch/
  
  Need help? http://forum.ardumote.com
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  ELECHOUSE_cc1101.setChannel(0xAF);// set channel in hex 80khz steps. channel number is 175 = 0xAF = 433,92Mhz / 315Mhz 
  ELECHOUSE_cc1101.Init(F_433);    // set frequency - F315, F_433, F_868, F_965 MHz
 
  // Transmitter is connected to Arduino Pin #2  
  mySwitch.enableTransmit(2);

  // cc1101 set Transmit on
  ELECHOUSE_cc1101.SetTx()
  
  // set pulse length.
  mySwitch.setPulseLength(360);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the channel (a, b, c, d)
  // The second parameter represents the device number
  // 
  // In this example it's family 'd', device #2 
  mySwitch.switchOn('d', 2);

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff('d', 2);
  
  // Wait another second
  delay(1000);
  
}
