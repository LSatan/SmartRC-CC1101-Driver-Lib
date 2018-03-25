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

//CC1101 Settings:  
char mhz = F_433;                  // Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx.
char ch = 175;                    // set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
char dbm = 10;                   // set Transmit Power (default = 10 dbm) 10 dbm, 7 dbm, 5 dbm, 0 dbm, -10 dbm, -15 dbm, -20 dbm, -30 dbm.
char RxBW = 1;                  // set Recive filter bandwidth (default = 58khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
char Chsp = 80;                // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
char freq2 = 0x10;            // set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
char freq1 = 0x27;           // set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
char freq0 = 0x62;          // set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.

const int buttonPin = 4;       // set button on pin D4
int buttonState = 0;          // button state
int ccReset = 0;             // reset state for Receive
int long value = 5393;      // int to save value
int bits = 24;             // int to save bit number
int prot = 1;             // int to save Protocol number

void setup() {
  Serial.begin(9600);

//ELECHOUSE_cc1101.RxBW(RxBW);           // set Recive filter bandwidth
//ELECHOUSE_cc1101.setdbm(dbm);         // set Transmit Power
//ELECHOUSE_cc1101.setChsp(Chsp);      // set Channle spacing
//ELECHOUSE_cc1101.freq2(freq2);      // set your own frequency Parameters Freq2
//ELECHOUSE_cc1101.freq1(freq1);     // set your own frequency Parameters Freq1
//ELECHOUSE_cc1101.freq0(freq0);    // set your own frequency Parameters Freq0
//ELECHOUSE_cc1101.setChannel(ch); // set channel
  ELECHOUSE_cc1101.Init(mhz);     // set frequency

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

  ELECHOUSE_cc1101.SetSres();             // Reset the cc1101
//ELECHOUSE_cc1101.RxBW(RxBW);           // set Recive filter bandwidth
//ELECHOUSE_cc1101.setdbm(dbm);         // set Transmit Power
//ELECHOUSE_cc1101.setChsp(Chsp);      // set Channle spacing
//ELECHOUSE_cc1101.freq2(freq2);      // set your own frequency Parameters Freq2
//ELECHOUSE_cc1101.freq1(freq1);     // set your own frequency Parameters Freq1
//ELECHOUSE_cc1101.freq0(freq0);    // set your own frequency Parameters Freq0
//ELECHOUSE_cc1101.setChannel(ch); // set channel
  ELECHOUSE_cc1101.Init(mhz);     // set frequency
  delay(100);                    // wait for reinit
  mySwitch.disableTransmit();   // set Transmit off
  mySwitch.enableReceive(0);   // Receiver on
  ELECHOUSE_cc1101.SetRx();   // set Recive on
  ccReset = 1;               // after reinit jump to recive part
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

