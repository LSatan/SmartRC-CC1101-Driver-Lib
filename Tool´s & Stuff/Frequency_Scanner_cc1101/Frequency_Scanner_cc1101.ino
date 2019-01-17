/*
This demo should show what is possible with the CC1101 module.
Detects the transmission frequency of your remote control.
Scanning in a range of 1MHZ takes 2 seconds.
During the scanning process the transmitter has to send continuously.
The distance between remote control and cc1101 should be between 5cm - 30cm.
If several frequencies are output, pay attention to repetition of the frequencies.
Starts when a signal has been detected. (Responds to every signal!)
If you have a lot of radio garbage in your environment it will scan permanently!
Does not work with car key.
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>

float Start = 433.00;   //Start frequency can be changed.
float Stop = 434.00;    //Stop frequency can be changed.

byte value=0;
byte rssi=0;
int pin; // int for Receive pin.
float Save;
float freq;
int maxRssi;
int step;
boolean scanning = false;
boolean waiting = false;
float lastTime = 0;

void setup() {
  Serial.begin(9600);

#ifdef ESP32
pin = 4;  // for esp32! Receiver on GPIO pin 4. 
#elif ESP8266
pin = 4;  // for esp8266! Receiver on pin 4 = D2.
#else
pin = 2;  // for Arduino! Receiver on interrupt 0 => that is pin #2
#endif    

//CC1101 Settings:
  ELECHOUSE_cc1101.Init(PA10);    // must be set to initialize the cc1101! set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30.
  ELECHOUSE_cc1101.SetRx();      // set Recive on
}
void loop() {
if (scanning == false) {
if (step == 0){
    ELECHOUSE_cc1101.setRxBW(16);
    freq = Start;
    step = 1;
}
if (step == 1){
    freq = freq + 0.2;
    ELECHOUSE_cc1101.SetRx(freq);
    delay(20);
if (freq >= Stop){
    freq = Start;
}
if (!waiting){
if (digitalRead(pin) == HIGH){
    lastTime = micros();
    waiting = true;
}
}else{
if ((micros() - lastTime > 3000) && (digitalRead(pin) == LOW)){
    waiting = false;
    scanning = true;
    step = 2;
    lastTime = micros();       
}else{
    waiting = false;
}
}
}
}else{
if (step == 2){
    freq = Start;
    ELECHOUSE_cc1101.setRxBW(1);
    ELECHOUSE_cc1101.SetRx(freq);
    delay(20);
    Serial.println("Start");
    getRssi();
    maxRssi = value;
    step = 3;  
}
if (freq >= Stop){
if (Save > 0){
    Serial.print("Frequenz: ");
    Serial.print(Save);
    Serial.println(" mhz");
    Serial.print("MaxRSSI: ");
    Serial.println(maxRssi);
    Save = 0;
    maxRssi = 0;
    step = 0;
    scanning = false;
    delay(100);
}else{
    Serial.println("Nothing! Scan again!");
    Save = 0;
    maxRssi = 0;
    step = 0;
    scanning = false;
    delay(100); 
}
}
if (freq < Stop && step == 3){
    freq = freq + 0.01;
    ELECHOUSE_cc1101.SetRx(freq);
    delay(20);
    getRssi();
if (maxRssi < value){
    maxRssi = value;
    Save = freq;
}
}
}
}
void getRssi(){
rssi=(ELECHOUSE_cc1101.SpiReadStatus(CC1101_RSSI));
if (rssi >= 128)
{
value = (255 - rssi)/2+74;
}
else
{
value = rssi/2+74;
}
}
