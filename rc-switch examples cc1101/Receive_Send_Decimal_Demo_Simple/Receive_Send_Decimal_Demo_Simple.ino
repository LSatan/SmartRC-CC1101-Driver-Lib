/*
  Simple example for Receiving and Transmit decimal code for cc1101
  
  Notes: Arduino only!!!
  
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
int puls = 320;          // int to save pulse length

void setup() {
  Serial.begin(9600);

//ELECHOUSE_cc1101.setRxBW(16);          // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChsp(80);         // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
//ELECHOUSE_cc1101.freq2(0x10);        // set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq1(0x27);       // set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq0(0x62);      // set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.setChannel(175); // set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
  ELECHOUSE_cc1101.Init(F_433);    // Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx. set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30. example: F_433_PA10. (default = PA0)


  pinMode(buttonPin, INPUT);     // set pin4 as input

  
}
void loop() {

   buttonState = digitalRead(buttonPin);     // read digital pin and save the state to int


   if (buttonState == HIGH) {     // the button is pressed set transmit on

  ccReset = 0;                  // set resetstate to 0 for next reinit to Recive
  ELECHOUSE_cc1101.SetTx();     // set Transmit on
  mySwitch.disableReceive();    // Receiver off
  mySwitch.enableTransmit(2);   // Transmit on

  mySwitch.setRepeatTransmit(1); // transmission repetitions.
  mySwitch.setPulseLength(puls); // send Received Delay
  mySwitch.setProtocol(prot); // send Received Protocol
  mySwitch.send(value, bits); // send Received value/bits

    Serial.print("Transmit ");
    Serial.print( value );
    Serial.print(" / ");
    Serial.print( bits );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( prot );
    Serial.print(" Delay: ");    
    Serial.println( puls );      
   }

   
  if (buttonState == LOW && ccReset == 0){  //the button is not pressed. set cc1101 to Receive.
    
  ELECHOUSE_cc1101.SetRx();      // set Recive on
  mySwitch.disableTransmit();   // set Transmit off
  mySwitch.enableReceive(0);   // Receiver on
  ccReset = 1;
   }

  if (buttonState == LOW && ccReset == 1) {   //the button is not pressed and reset is finish, receive values. 



  if (mySwitch.available()){
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( mySwitch.getReceivedProtocol() );
    Serial.print(" Delay: ");    
    Serial.println( mySwitch.getReceivedDelay() );

    value =  mySwitch.getReceivedValue();        // save received Value
    bits = mySwitch.getReceivedBitlength();     // save received Bitlength
    prot = mySwitch.getReceivedProtocol();     // save received Protocol
    puls =  mySwitch.getReceivedDelay();      // save received pulse length
    
    mySwitch.resetAvailable();
   
   }
   }

}
