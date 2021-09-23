/*
 * Demo for RF remote switch receiver. 
 * This example is for the new KaKu / Home Easy type of remotes!
 *
 * For details, see NewRemoteReceiver.h!
 *
 * With this sketch you can control a LED connected to LED_BUILTIN pin,
 * after the sketch learned the code. After start, the LED starts to blink,
 * until a valid code has been received. The led stops blinking. Now you
 * can control the LED with the remote.
 *
 * Note: only unit-switches are supported in this sketch, no group or dim.
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

#if !defined LED_BUILTIN
  #if defined ESP8266 || defined ESP32
    #define LED_BUILTIN 2
  #else
    #define LED_BUILTIN 13
  #endif
  #define VALUE_TO_STRING(x) #x
  #define VALUE(x) VALUE_TO_STRING(x)
  #pragma message("SETTING LED_BUILTIN TO PIN " VALUE(LED_BUILTIN))
#endif

boolean codeLearned = false;
unsigned long learnedAddress;
byte learnedUnit;

void setup() {
    

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
 
  ELECHOUSE_cc1101.SetRx();    // set Receive on
  
  // LED-pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Init a new receiver on interrupt 0 (= digital pin 2) for Arduino Uno/Nano.
  // On ESP8266 and ESP32 use on GPIO 4 = digital pin depends board.
  // Review file "pins_arduino.h" of your variant:
  //   https://github.com/esp8266/Arduino/tree/master/variants
  //   https://github.com/espressif/arduino-esp32/tree/master/variants
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  //
  // Calls the callback "processCode" after 2 identical codes have been received
  // in a row. (thus, keep the button pressed for a moment).

  #if defined ESP8266 || defined ESP32
    NewRemoteReceiver::init(4, 2, processCode);
  #else
    NewRemoteReceiver::init(0, 2, processCode);
  #endif
}

void loop() {
  // Blink led until a code has been learned
  if (!codeLearned) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}

// Callback function is called only when a valid code is received.
void processCode(NewRemoteCode receivedCode) {
  // A code has been received.
  // Do we already know the code?
  if (!codeLearned) {
    // No! Let's learn the received code.
    learnedAddress = receivedCode.address;
    learnedUnit = receivedCode.unit;
    codeLearned = true;    
  } else {
    // Yes!    
    // Is the received code identical to the learned code?
    if (receivedCode.address == learnedAddress && receivedCode.unit == learnedUnit) {
      // Yes!
      // Switch the LED off if the received code was "off".
      // Anything else (on, dim, on_with_dim) will switch the LED on.
      if (receivedCode.switchType == NewRemoteCode::off) {
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }
}
