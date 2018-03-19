/*
  Simple example for Receiving and Transmit decimal code for cc1101
  https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


const int buttonPin = 4;       // set button on pin D4
int buttonState = 0;          // button state
int ccReset = 0;             // reset state for Receive
int long value = 5393;      // int to save value
int bits = 24;             // int to save bit number
int prot = 1;             // int to save Protocol number

void setup() {
  Serial.begin(9600);
  
 ELECHOUSE_cc1101.Init(F_433);  // set frequency - F_433, F_868, F_965 MHz
 pinMode(buttonPin, INPUT);     // set pin4 as input

  
}
void loop() {

   buttonState = digitalRead(buttonPin);     // read digital pin and save the state to int


   if (buttonState == HIGH) {     // the button is pressed set transmit on
  
  ccReset = 0;                   // set resetstate to 0 for next reinit to Recive
  mySwitch.disableReceive();    // Receiver off
  mySwitch.enableTransmit(2);  // Transmit on
  ELECHOUSE_cc1101.SetTx();   // set Transmit on

  mySwitch.setProtocol(prot); // send Recived Protocol
  mySwitch.send(value, bits); // send Recived value/bits

    Serial.print("Transmit ");
    Serial.print( value );
    Serial.print(" / ");
    Serial.print( bits );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( prot );
    
    
    }

   
   if (buttonState == LOW && ccReset == 0) {  //the button is not pressed reset cc1101 for Recive

  ELECHOUSE_cc1101.SetSres();     // Reset the cc1101
  ELECHOUSE_cc1101.Init(F_433);  // set frequency - F_433, F_868, F_965 MHz
  delay(100);                   // wait for reinit
  mySwitch.disableTransmit();  // set Transmit off
  mySwitch.enableReceive(0);  // Receiver on
  ELECHOUSE_cc1101.SetRx();  // set Recive on
  ccReset = 1;              // after reinit jump to recive part
}


   if (buttonState == LOW && ccReset == 1) {   //the button is not pressed and reset is finish, recive values. 

  if (mySwitch.available()){
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );

    value =  mySwitch.getReceivedValue();        // save recived Value
    bits = mySwitch.getReceivedBitlength();     // save recived Bitlength
    prot = mySwitch.getReceivedProtocol();     // save recived Protocol

    mySwitch.resetAvailable();

   }
   }
   }

