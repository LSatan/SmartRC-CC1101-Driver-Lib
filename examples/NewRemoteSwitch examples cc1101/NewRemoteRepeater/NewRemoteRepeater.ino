/*
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 *
 * When run, this sketch waits for a valid code from a new-style the receiver,
 * decodes it, and retransmits it after 1 seconds.
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
 *  Written by Roman.(Arduino Forum) THX!
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 */

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>

int pinTx;        // int for Transmit digital pin.
int pinRx;        // int for for Receive interrupt or GPIO number.
int Repeats = 2;  // int for number of times the same code must be received in a row before the callback is calles.
int Anz = 3;      // number of retransmissions
int Tw = 1000;    // Wait Miliseconds before sending

int debug = 1; // Debugmode ein (1)/aus(0)

// Global vars to store received code parameters for retransmitt it
unsigned int period;
unsigned long address;
unsigned long groupBit; 
unsigned long unit;
unsigned long switch_Type;
boolean dimLevelPresent;
byte dimLevel;
boolean codeLearned = false;  // Flag to set a valid learned code has been received

void setup() {
  Serial.begin(115200);

  #ifdef ESP32
  pinRx = 4; pinTx = 14;  // for esp32! Receiver on GPIO 4 and Transmit on GPIO 14 (digital pins depends board).
  #elif ESP8266
  pinRx = 4; pinTx = 5;  // for esp8266! Receiver on GPIO 4 and Transmit on GPIO 5 (digital pins depends board).
  #else
  pinRx = 0; pinTx = 6;  // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on digital pin 6.
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
  // See example ShowReceivedCode for info on this
  NewRemoteReceiver::init(pinRx, Repeats, ReTrans);
  if (debug == 1) {Serial.println("Receiver initialized... ");}
}

void loop() {

  if (codeLearned) {

    // Clear flag
    codeLearned = false;

    if (debug == 1) {
      // Print the received code.
      Serial.print("Receiver: ");
      Serial.print("Code: ");
      Serial.print(address);
      Serial.print(" unit: ");
      Serial.print(unit);
      Serial.print(" switchType: ");
      Serial.print(switch_Type);
      Serial.print(" Period: ");
      Serial.print(period);
      Serial.print(" groupBit: ");
      Serial.println(groupBit);
      if (dimLevelPresent){
        Serial.print(" dimLevel: ");
        Serial.println(dimLevel);    
      }
    }

    if (debug == 1) {Serial.print("Wait... ");}
    
    // Wait 1 seconds before sending.
    delay(Tw);

    ELECHOUSE_cc1101.SetTx();  // set Transmit on (Receive off)

    // Create a new transmitter with the received address and period, use digital pin as output pin
    NewRemoteTransmitter transmitter(address, pinTx, period, Anz);
    if (debug == 1) {Serial.print("Send: Addr " + String(address) + " unit " +  String(unit)+" "+ String(switch_Type)+", period: "+String(period)+" " );}
  
    // On/Off signal received
    bool isOn = switch_Type == NewRemoteCode::on;

    if (groupBit) {
      // Send to the group
      transmitter.sendGroup(isOn);
    }
    else {
      // Send to a single unit
      transmitter.sendUnit(unit, isOn);
    }

    ELECHOUSE_cc1101.setSidle();  // set Idle (Transmit off and Receive off)

    if (debug == 1) {Serial.println("Receiver enable!");}
    NewRemoteReceiver::enable();

    ELECHOUSE_cc1101.SetRx();  // set Receive on

  }
}

void ReTrans(unsigned int _period, unsigned long _address, unsigned long _groupBit, unsigned long _unit, unsigned long _switchType, boolean _dimLevelPresent, byte _dimLevel){  // Disable the receiver; otherwise it might pick up the retransmit as well.

  if (debug == 1) {Serial.print("Send: Receiver disable... ");}
  NewRemoteReceiver::disable();

  // Store received code parameters for retramitt it in loop()
  period = _period;
  address = _address;
  groupBit = _groupBit; 
  unit = _unit;
  switch_Type = _switchType;
  dimLevelPresent = _dimLevelPresent;
  dimLevel = _dimLevel;

  // Set valid learned code has been received
  codeLearned = true;
  
}
