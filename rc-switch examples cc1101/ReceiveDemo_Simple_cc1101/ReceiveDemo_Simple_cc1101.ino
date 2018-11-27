/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

int pin; // int for Receive pin.

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);

#ifdef ESP32
pin = 4;  // for esp32! Receiver on GPIO pin 4. 
#elif ESP8266
pin = 4;  // for esp8266! Receiver on pin 4 = D2.
#else
pin = 0;  // for Arduino! Receiver on interrupt 0 => that is pin #2
#endif    

//CC1101 Settings:                (Settings with "//" are optional!)
//ELECHOUSE_cc1101.setRxBW(16);     // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.Init(PA10);    // must be set to initialize the cc1101! set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30.

  mySwitch.enableReceive(pin);  // Receiver on

  ELECHOUSE_cc1101.SetRx();  // set Recive on
  
}
void loop() {
 
  if (mySwitch.available()){
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );

    mySwitch.resetAvailable();
  }
}
