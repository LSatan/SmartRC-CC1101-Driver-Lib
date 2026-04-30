/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to an attachable interruption pin. See below.
* 
* 
*Detected codes example:
 code: 8233372 Period: 273
 unit: 1
 groupBit: 0
 switchType: 0

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
* Set-up: connect CC1101 GDO2 pin to an attachable interruption pin. See below.
* 
*  https://github.com/1technophile/NewRemoteSwitch
*  https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
*  ----------------------------------------------------------
*  Mod by Little Satan. Have Fun!
*  ----------------------------------------------------------
* 
*/
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <NewRemoteReceiver.h>

int pin; // int for Receive interrupt or GPIO number.

void setup() {
  Serial.begin(115200);

// Initialize receiver on interrupt 0 (= digital pin 2) for Arduino Uno. 
// On ESP8266 and ESP32 use on GPIO 4 = digital pin depends board. 
// Review file "pins_arduino.h" of your variant:
//   https://github.com/esp8266/Arduino/tree/master/variants
//   https://github.com/espressif/arduino-esp32/tree/master/variants
//
#ifdef ESP32
pin = 4;  // for esp32! Receiver on GPIO 4 (digital pin depends board).
#elif ESP8266
pin = 4;  // for esp8266! Receiver on GPIO 4 (digital pin depends board).
#else
pin = 0;  // for Arduino! Receiver on interrupt 0 => that is pin #2
#endif  

  if (ELECHOUSE_cc1101.getCC1101()){       // Check the CC1101 Spi connection.
  Serial.println("Connection OK");
  }else{
  Serial.println("Connection Error");
  }

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
 
  ELECHOUSE_cc1101.SetRx();     // set Receive on
  
  NewRemoteReceiver::init(pin, 2, showCode);
  Serial.println("Receiver initialized");    
}

void loop() {

}

// Callback function is called only when a valid code is received.
void showCode(unsigned int period, unsigned long address, unsigned long groupBit, unsigned long unit, unsigned long switchType, boolean dimLevelPresent, byte dimLevel) {

  // Print the received code.
  Serial.print("Code: ");
  Serial.print(address);
  Serial.print(" Period: ");
  Serial.println(period);
  Serial.print(" unit: ");
  Serial.println(unit);
  Serial.print(" groupBit: ");
  Serial.println(groupBit);
  Serial.print(" switchType: ");
  Serial.println(switchType);

  if (dimLevelPresent){
    Serial.print(" dimLevel: ");
    Serial.println(dimLevel);    
  }

}
