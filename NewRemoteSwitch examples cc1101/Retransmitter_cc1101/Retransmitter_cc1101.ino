/*
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 *
 * When run, this sketch waits for a valid code from a new-style the receiver,
 * decodes it, and retransmits it after 5 seconds.
 * 
 *  Notes: Arduino only!!!
 *  
 *  https://github.com/1technophile/NewRemoteSwitch
 *  https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
 *  ----------------------------------------------------------
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 */

#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>

void setup() {

//CC1101 Settings:                (Settings with "//" are optional!)
//ELECHOUSE_cc1101.setRxBW(16);  // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(175);// set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
//ELECHOUSE_cc1101.setChsp(80); // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
//ELECHOUSE_cc1101.freq2(0x10); //set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq1(0x27); //set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq0(0x62); //set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.
  ELECHOUSE_cc1101.Init(F_433); //Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx. set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30. example: F_433_PA10. (default = PA10)
  ELECHOUSE_cc1101.SetRx();  // set Recive on 
  
  // See example ShowReceivedCode for info on this
  NewRemoteReceiver::init(0, 2, retransmitter);
}

void loop() {
}

void retransmitter(NewRemoteCode receivedCode) {
  // Disable the receiver; otherwise it might pick up the retransmit as well.
  NewRemoteReceiver::disable();

  // Need interrupts for delay()
  interrupts();

  // Wait 5 seconds before sending.
  delay(5000);

  // Create a new transmitter with the received address and period, use digital pin as output pin

  ELECHOUSE_cc1101.SetTx();  // set Transmit on
  NewRemoteTransmitter transmitter(receivedCode.address, 6, receivedCode.period);

  if (receivedCode.switchType == NewRemoteCode::dim || 
    (receivedCode.switchType == NewRemoteCode::on && receivedCode.dimLevelPresent)) {
    // Dimmer signal received

    if (receivedCode.groupBit) {
      transmitter.sendGroupDim(receivedCode.dimLevel);
    } 
    else {
      transmitter.sendDim(receivedCode.unit, receivedCode.dimLevel);
    }
  } 
  else {
    // On/Off signal received
    bool isOn = receivedCode.switchType == NewRemoteCode::on;

    if (receivedCode.groupBit) {
      // Send to the group
      transmitter.sendGroup(isOn);
    } 
    else {
      // Send to a single unit
      transmitter.sendUnit(receivedCode.unit, isOn);
    }
  }
  ELECHOUSE_cc1101.SetRx();  // set Recive on
  NewRemoteReceiver::enable();
}

