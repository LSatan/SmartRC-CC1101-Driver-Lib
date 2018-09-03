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

int esp; // for ESP8266 & Arduino setting. 
int pin; // int for Transmit pin.

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);

#ifdef ESP32
esp = 2; pin = 2;  // for esp32! Transmit on GPIO pin 2.
#elif ESP8266
esp = 1; pin = 5;  // for esp8266! Transmit on pin 5 = D1
#else
esp = 0; pin = 6;  // for Arduino! Transmit on pin 6.
#endif 

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.setESP8266(esp);    // esp8266 & Arduino SPI pin settings. Don´t change this line!
//ELECHOUSE_cc1101.setRxBW(16);       // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(1);    // set channel. steps from Channle spacing.0 - 255 default channel number is 1 for basic frequency.
//ELECHOUSE_cc1101.setChsp(50);     // set Channle spacing (default = 50khz) you can set 25,50,80,100,150,200,250,300,350,405.
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.Init(PA10);    // must be set to initialize the cc1101! set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30.
 
  // Transmitter on 
  mySwitch.enableTransmit(pin);

  // cc1101 set Transmit on
  ELECHOUSE_cc1101.SetTx();
  
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
