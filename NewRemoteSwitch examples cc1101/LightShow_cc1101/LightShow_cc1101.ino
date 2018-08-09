/**
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 * Connect the transmitter to digital pin 11.
 *
 * This sketch demonstrates the use of the NewRemoteTransmitter class.
 *
 * When run, this sketch switches some pre-defined devices on and off in a loop.
 *
 * NOTE: the actual receivers have the address and group numbers in this example
 * are only for demonstration! If you want to duplicate an existing remote, please
 * try the "retransmitter"-example instead.
 * 
 * To use this actual example, you'd need to "learn" the used code in the receivers
 * This sketch is unsuited for that.
 * 
 *  Notes: Arduino only!!!
 * 
 *  https://github.com/1technophile/NewRemoteSwitch
 *  https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
 *  ----------------------------------------------------------
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 * 
 */
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <NewRemoteTransmitter.h>

void setup() {

//CC1101 Settings:                (Settings with "//" are optional!)
//ELECHOUSE_cc1101.setRxBW(16);       // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(1);    // set channel. steps from Channle spacing.0 - 255 default channel number is 1 for basic frequency.
//ELECHOUSE_cc1101.setChsp(50);     // set Channle spacing (default = 50khz) you can set 25,50,80,100,150,200,250,300,350,405.
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.Init(PA10);    // must be set to initialize the cc1101! set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30.
  
  ELECHOUSE_cc1101.SetTx();     // cc1101 set Transmit on
}

void loop() { 

  // Create a transmitter on address 123, using digital pin 6 to transmit, 
  // with a period duration of 260ms (default), repeating the transmitted
  // code 2^3=8 times.
  NewRemoteTransmitter transmitter(123, 6, 260, 3);
   
  // Switch unit 2 off
  transmitter.sendUnit(2, false);

  // Switch all devices in the group off
  transmitter.sendGroup(false);

  // Set unit 1 to dim-level 3 (range 0-15)
  transmitter.sendDim(1, 3);  

  // Wait 5 seconds
  delay(5000);
  

  // Switch unit 2 on
  transmitter.sendUnit(2, true);

  // Switch all devices in the group on
  transmitter.sendGroup(true);

  // Set unit 1 to dim-level 15, full brightness.
  transmitter.sendDim(1, 15);

  // Wait 5 seconds
  delay(5000);
}
