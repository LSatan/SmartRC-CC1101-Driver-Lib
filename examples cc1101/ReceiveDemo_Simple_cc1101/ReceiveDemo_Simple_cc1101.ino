/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  
  ELECHOUSE_cc1101.Init(F_433);  // set frequency - F_433, F_868, F_965 MHz
  
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

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
