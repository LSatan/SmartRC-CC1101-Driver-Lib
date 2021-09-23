/**
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 * Connect the transmitter to digital pin 6.
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
 * Works on any Arduino AVR, ESP8266 or ESP32. NewRemoteSwitch library commit:  
 * c3ca4cb (Sep 19, 2021) or later, required to work properly on ESP8266 and ESP32.
 * 
 * Set-up: connect CC1101 SPI:
 *                          SCK  MISO MOSI  SS
 * Arduino UNO/Nano  (pin)   13   12   11   10
 * Arduino Mega2560  (pin)   52   50   51   53
 * Espressif ESP8266 (GPIO)  14   12   13   15  (digital pins depends board)
 * Espressif ESP32   (GPIO)  18   19   23    5  (digital pins depends board)
 * 
 * Set-up: connect CC1101 GDO0 pin to a digital pin to transmit. See below.
 * 
 *  https://github.com/1technophile/NewRemoteSwitch
 *  https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 *  ----------------------------------------------------------
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 * 
 */
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <NewRemoteTransmitter.h>

int pin; // int for Transmit pin.

void setup() {

#ifdef ESP32
pin = 14;  // for esp32! Transmit on GPIO 14 (digital pin depends board).
#elif ESP8266
pin = 5;  // for esp8266! Transmit on GPIO 5 (digital pin depends board).
#else
pin = 6;  // for Arduino! Transmit on digital pin 6 (Arduino).
#endif 

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.

}

void loop() { 

  ELECHOUSE_cc1101.SetTx();     // cc1101 set Transmit on

  // Create a transmitter on address 123, using a digital pin to transmit, 
  // with a period duration of 260ms (default), repeating the transmitted
  // code 2^3=8 times.
  NewRemoteTransmitter transmitter(123, pin, 260, 3);
   
  // Switch unit 2 off
  transmitter.sendUnit(2, false);

  // Switch all devices in the group off
  transmitter.sendGroup(false);

  // Set unit 1 to dim-level 3 (range 0-15)
  transmitter.sendDim(1, 3);  

  ELECHOUSE_cc1101.setSidle();  // set Idle (Transmit off)

  // Wait 5 seconds
  delay(5000);
  
  ELECHOUSE_cc1101.SetTx();     // cc1101 set Transmit on

  // Switch unit 2 on
  transmitter.sendUnit(2, true);

  // Switch all devices in the group on
  transmitter.sendGroup(true);

  // Set unit 1 to dim-level 15, full brightness.
  transmitter.sendDim(1, 15);

  ELECHOUSE_cc1101.setSidle();  // cc1101 set Idle (Transmit off)

  // Wait 5 seconds
  delay(5000);
}
