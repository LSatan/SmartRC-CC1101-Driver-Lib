/*
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 *
 * When run, this sketch waits for a valid code from a new-style the receiver,
 * decodes it, and retransmits it after 5 seconds.
 * 
 * Works on any Arduino AVR, ESP8266 or ESP32. NewRemoteSwitch library commit:  
 * c3ca4cb (Sep 19, 2021) or later, required to work properly on ESP8266 and ESP32.
 * 
 * To prevent kernel panic for interrupt wdt timeout on ESP8266 and ESP32, 
 * the retransmission of the received code is done in loop() function.
 * 
 * Set-up: connect CC1101 SPI:
 *                          SCK  MISO MOSI  SS
 * Arduino UNO/Nano  (pin)   13   12   11   10
 * Arduino Mega2560  (pin)   52   50   51   53
 * Espressif ESP8266 (GPIO)  14   12   13   15  (digital pins depends board)
 * Espressif ESP32   (GPIO)  18   19   23    5  (digital pins depends board)
 * 
 * Set-up: connect CC1101 GDO2 pin to an attachable interruption pin. See below.
 * Set-up: connect CC1101 GDO0 pin to a digital pin to transmit. See below.
 *  
 *  https://github.com/1technophile/NewRemoteSwitch
 *  https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 *  ----------------------------------------------------------
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 */

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>

NewRemoteCode receivedCode;   // Global var to store received code for retramitt it
boolean codeLearned = false;  // Flag to set a valid learned code has been received

void setup() {

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.

  ELECHOUSE_cc1101.SetRx();  // set Receive on 
  
  // Initialize receiver on interrupt 0 (= digital pin 2) for Arduino Uno. 
  // On ESP8266 and ESP32 use on GPIO 4 = digital pin depends board. 
  // Review file "pins_arduino.h" of your variant:
  //   https://github.com/esp8266/Arduino/tree/master/variants
  //   https://github.com/espressif/arduino-esp32/tree/master/variants
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  //
  // Calls the callback "retransmitter" after 2 identical codes have been received 
  // in a row. (thus, keep the button pressed for a moment).
  
  #if defined ESP8266 || defined ESP32
    NewRemoteReceiver::init(4, 2, retransmitter);
  #else
    NewRemoteReceiver::init(0, 2, retransmitter);
  #endif
}

void loop() {

  if (codeLearned) {

    // Clear flag
    codeLearned = false;

    // Wait 5 seconds before sending.
    delay(5000);

    ELECHOUSE_cc1101.SetTx();  // set Transmit on (Receive off)

    // Create a new transmitter with the received address and period, use a digital pin as output pin
    #if defined ESP32
    NewRemoteTransmitter transmitter(receivedCode.address, 14, receivedCode.period); // Transmit on GPIO 14 (digital pin depends board)
    #elif defined ESP8266 
    NewRemoteTransmitter transmitter(receivedCode.address, 5, receivedCode.period);  // Transmit on GPIO 5 (digital pin depends board)
    #else
    NewRemoteTransmitter transmitter(receivedCode.address, 6, receivedCode.period);  // Transmit on digital pin 6 (Arduino)
    #endif

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

    ELECHOUSE_cc1101.setSidle();  // set Idle (Receive off and Transmit off)

    NewRemoteReceiver::enable();

    ELECHOUSE_cc1101.SetRx();  // set Receive on (Transmit off)

  }
}

void retransmitter(NewRemoteCode retransmitterCode) {

  // Disable the receiver; otherwise it might pick up the retransmit as well.
  NewRemoteReceiver::disable();

  // Store received code for retramitt it in loop()
  receivedCode = retransmitterCode;

  // Set valid learned code has been received
  codeLearned = true;

}
