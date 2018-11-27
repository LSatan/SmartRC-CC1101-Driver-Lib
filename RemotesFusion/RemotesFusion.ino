/*
Welcome to RemotesFusion
RemotesFusion is a webserver application for Esp and cc1101 module.
That can controll your Remote controlled rf devices whith smartphone.
https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
You need Rc-Switch:  https://github.com/sui77/rc-switch
and NewRemoteSwitch: https://github.com/1technophile/NewRemoteSwitch
Support me with a donation: https://www.paypal.me/LittleSatan666
THANK YOU!
Created by Little_S@tan.
*/

#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>
#include <WiFiClient.h> 
#include <EEPROM.h>
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif 


RCSwitch mySwitch = RCSwitch();
WiFiServer server(80);


//Wifi
char CLssid[65];
char CLpassword[65];
char *APssid     = "Remotes_Fusion";
char APpassword[65] = "Fusion2018";
char *clhost = "remotesfusion";
String filter;
String save;
String header;
int timeout;


//CC1101
int pinRx;
int pinTx;
float MHZ = 433.92;
byte RxBW = 16;


//RC-Switch 
int long value;
int bits;
int prot;
int puls;
int repRcs = 15;

//NewRemoteSwitch
int long Address;
int Period;
int Unit;
int GroupBit;
int SwitchType;
int repNrs;
int DimLevel;
bool Switch;

//Serial Monitor / Data evaluation.
int repeater;
int receiver;
int fusion;
int rx;
int show_wifi;
int cc1101;
int addr = 0;
int count;
String input;
String int_1;
String int_2;
String int_3;
String int_4;
String int_5;
String int_6;
String int_7;
String int_8;
long boundLow;
long boundHigh;
const char delimiter = ',';



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
Serial.begin(115200);
EEPROM.begin(512);

 do{
    byte z = EEPROM.read(addr);
 if (z == 255){addr = 196;}else{
    char a = char(z);
    save = save + a;
    addr = addr +1;
 }
  } while(addr < 195);

    addr = 0;

    boundLow = save.indexOf(delimiter);
    int_1 = save.substring(0, boundLow);
    
    boundHigh = save.indexOf(delimiter, boundLow+1);
    int_2 = save.substring(boundLow+1, boundHigh);
    
    boundLow = save.indexOf(delimiter, boundHigh+1);
    int_3 = save.substring(boundHigh+1, boundLow);

if (int_3.length() > 1){

int_1.toCharArray(CLssid, 65);
int_2.toCharArray(CLpassword, 65);
int_3.toCharArray(APpassword, 65);
}
save = "";
int_1 = "";
int_2 = "";
int_3 = "";


//CC1101 Settings:
addr = 212;
 do{
    byte z = EEPROM.read(addr);
 if (z == 255){addr = 233;}else{
    char a = char(z);
    save = save + a;
    addr = addr +1;
 }
  } while(addr < 233);

    addr = 0;

    boundLow = save.indexOf(delimiter);
    int_1 = save.substring(0, boundLow);
    
    boundHigh = save.indexOf(delimiter, boundLow+1);
    int_2 = save.substring(boundLow+1, boundHigh);
    
    boundLow = save.indexOf(delimiter, boundHigh+1);
    int_3 = save.substring(boundHigh+1, boundLow);


#ifdef ESP32
pinRx = 4; pinTx = 2;
#elif ESP8266
pinRx = 4; pinTx = 5;
#endif 

if (int_1.length() > 1){
MHZ = int_1.toFloat();
if (int_2 == "1"){RxBW = 1;}if (int_2 == "2"){RxBW = 2;}if (int_2 == "3"){RxBW = 3;}if (int_2 == "4"){RxBW = 4;}
if (int_2 == "5"){RxBW = 5;}if (int_2 == "6"){RxBW = 6;}if (int_2 == "7"){RxBW = 7;}if (int_2 == "8"){RxBW = 8;}
if (int_2 == "9"){RxBW = 9;}if (int_2 == "10"){RxBW = 10;}if (int_2 == "11"){RxBW = 11;}if (int_2 == "12"){RxBW = 12;}
if (int_2 == "13"){RxBW = 13;}if (int_2 == "14"){RxBW = 14;}if (int_2 == "15"){RxBW = 15;}if (int_2 == "16"){RxBW = 16;}
ELECHOUSE_cc1101.setRxBW(RxBW); 
ELECHOUSE_cc1101.setMHZ(MHZ);
if (int_3 == "PA10"){ELECHOUSE_cc1101.Init(PA10);}  if (int_3 == "PA7"){ELECHOUSE_cc1101.Init(PA7);}  
if (int_3 == "PA5"){ELECHOUSE_cc1101.Init(PA5);} if (int_3 == "PA0"){ELECHOUSE_cc1101.Init(PA0);} 
if (int_3 == "PA_10"){ELECHOUSE_cc1101.Init(PA_10);}  if (int_3 == "PA_15"){ELECHOUSE_cc1101.Init(PA_15);} 
if (int_3 == "PA_20"){ELECHOUSE_cc1101.Init(PA_20);} if (int_3 == "PA30"){ELECHOUSE_cc1101.Init(PA_30);}
}else{
ELECHOUSE_cc1101.setRxBW(RxBW);
ELECHOUSE_cc1101.setMHZ(MHZ);
ELECHOUSE_cc1101.Init(PA10);
}
save = "";
int_1 = "";
int_2 = "";
int_3 = "";

//Access Point
  Serial.println("//////////////////////////////////////////////");
  Serial.println("Configuring access point...");
  WiFi.softAP(APssid, APpassword);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");

//Client
#ifdef ESP8266
  WiFi.hostname(clhost);
#endif 
  Serial.println("Connecting to ");
  Serial.println(CLssid);
  WiFi.begin(CLssid, CLpassword);
#ifdef ESP32
  WiFi.setHostname(clhost);
#endif 
  while (timeout < 30) {
    delay(500);
    Serial.print(".");
  if (WiFi.status() == WL_CONNECTED){
    timeout = 31;    
  }
   timeout = timeout + 1;
  }
  timeout = 0;
  if (WiFi.status() == WL_CONNECTED){
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("CL IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("CL Hostname: ");
  Serial.println(clhost);
  server.begin();
}else{
  WiFi.disconnect();
  Serial.println("connection timeout!");
  Serial.println("WiFi not connected. AP Mode Only!");
}
Serial.println("//////////////////////////////////////////////");
Serial.println("CC1101 MHZ: ");
Serial.println(MHZ);
Serial.println("Remote Fusion Ready");
Serial.println("//////////////////////////////////////////////");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  WiFiClient client = server.available();
 
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            boundLow = header.indexOf("HTTP");
            input = header.substring(5, boundLow);
            input.trim();

            boundLow = input.indexOf(",");
            filter = input.substring(0, boundLow);
            filter.trim();
            delay(100);
            Serial.println(input);
            client.print(filter);
           
if (rx == 1 && Address > 0){
   client.print(Address);
   client.print(",");   
   client.print(Unit);
   client.print(",");
   client.print(GroupBit);
   client.print(",");
   client.print(SwitchType);
   client.print(",");
   client.print(Period);
   Address = 0;
   rx = 0;
}
if (rx == 1 && value > 0){
   client.print(value);
   client.print(",");   
   client.print(bits);
   client.print(",");
   client.print(prot);
   client.print(",");
   client.print(puls);
   value = 0;
   rx = 0;
}
if (save.length() > 1){
  client.print(save);
  Serial.print(save);
  save = "";
}
if (show_wifi == 1){
   client.print("wifi");
   client.print(",");  
   client.print(WiFi.softAPIP());
   client.print(",");
   client.print(APpassword);
   client.print(",");
   client.print(clhost);   
   client.print(",");
   client.print(CLssid);
   client.print(",");
   client.print(CLpassword);
   show_wifi = 0;
}
if (show_wifi == 2){
   client.print("saved");
   show_wifi = 0; 
}
if (show_wifi == 3){
   client.print("finish");
   show_wifi = 0; 
}
if (cc1101 == 1){
   client.print("saved");
   cc1101 = 0;
}
if (fusion == 1){
   client.print(",");
   client.print(repeater);
   client.print(",");
   client.print(receiver);
   fusion = 0;
}

       
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;
}
}
}

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//process received data

  if (repeater == 2){
    input = "txrcs";
  }
  if (repeater == 3){
    input = "txnrs";
  }
  if (input.length() > 0){

    boundLow = input.indexOf(delimiter);
    int_1 = input.substring(0, boundLow);
    
    boundHigh = input.indexOf(delimiter, boundLow+1);
    int_2 = input.substring(boundLow+1, boundHigh);
    
    boundLow = input.indexOf(delimiter, boundHigh+1);
    int_3 = input.substring(boundHigh+1, boundLow);
  
    boundHigh = input.indexOf(delimiter, boundLow+1);
    int_4 = input.substring(boundLow+1, boundHigh);

    boundLow = input.indexOf(delimiter, boundHigh+1);
    int_5 = input.substring(boundHigh+1, boundLow);

    boundHigh = input.indexOf(delimiter, boundLow+1);
    int_6 = input.substring(boundLow+1, boundHigh);

    boundLow = input.indexOf(delimiter, boundHigh+1);
    int_7 = input.substring(boundHigh+1, boundLow);
    
    int_8 = input.substring(boundLow+1);
  }

  if (input.length() > 0){

  if (int_1 == "scan"){ 
  esp_scan();
  }
  if (int_1 == "esp"){ 
  esp_settings();  
  }
  if (int_1 == "fusion"){  
   fusion_settings();      
  }                        
  if (int_1 == "cc1101"){ 
  cc1101_settings();
  }
  if (int_1 == "reconnect"){
  esp_reconnect();
  }
  if (int_1 == "rxrcs"){  
   input = "";
   rx = 1;
  receive_rcswitch();
 }
  if (int_1 == "rxnrs"){
   rx = 1;
  receive_newremoteswitch();
 }
  if (int_1 == "rxall"){ 
   input = "";
   rx = 0;
  receive_all();
 }
  if (int_1 == "txrcs"){
  send_rcswitch();
 }
  if (int_1 == "txnrs"){
  send_newremoteswitch();
}
  if (int_1 == "dimnrs"){
  dim_newremoteswitch();
}
}
  if (repeater == 1 || receiver == 1){
  receive_all();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void esp_scan() {
input = "";
save = "wlanwifi,";

//wifi
  delay(100);

  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
#ifdef ESP32
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
#elif ESP8266
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
#endif 
      save = save + (WiFi.SSID(i) + ",");
      delay(10);
    }
  }
  Serial.println("");
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void esp_settings() {
input = "";

if (int_2 == "esp"){
show_wifi = 1;  
  }else{
String store = int_2 + "," + int_3 + "," + int_4 + ",";
EEPROM.begin(512);
for (int i=0; i< 195; i++){
EEPROM.write(i, 255);
}

for(int i=0; i<store.length(); i++){
EEPROM.write(i, store[i]);
 }
EEPROM.commit();
 do{
    byte z = EEPROM.read(addr);
    char a = char(z);
    save = save + a;
    addr = addr +1;
  } while(addr < 195);

    addr = 0;

    boundLow = save.indexOf(delimiter);
    int_1 = save.substring(0, boundLow);
    
    boundHigh = save.indexOf(delimiter, boundLow+1);
    int_2 = save.substring(boundLow+1, boundHigh);
    
    boundLow = save.indexOf(delimiter, boundHigh+1);
    int_3 = save.substring(boundHigh+1, boundLow);


int_1.toCharArray(CLssid, 65);
int_2.toCharArray(CLpassword, 65);
int_3.toCharArray(APpassword, 65);

int_1 = "";
int_2 = "";
int_3 = "";
Serial.println("Saved to EEPROM");
save = "";
show_wifi = 2;
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fusion_settings() {
input = "";
if (int_2 == "get"){fusion = 1;}
else{
repeater = int_2.toInt();
receiver = int_3.toInt();
Serial.println("Fusion_Settings_OK");
}
if (repeater == 1 || receiver == 1){receive_all();}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cc1101_settings() {
input = "";
addr = 212;
String store = int_2 + "," + int_3 + "," + int_4 + ",";
for (int i=212; i< 232; i++){
EEPROM.write(i, 255);
}

for(int i=0; i<store.length(); i++){
EEPROM.write(addr, store[i]);
addr = addr +1; 
 }
EEPROM.commit();
addr = 212;
 do{
    byte z = EEPROM.read(addr);
    char a = char(z);
    save = save + a;
    addr = addr +1;
  } while(addr < 232);

    addr = 0;

    boundLow = save.indexOf(delimiter);
    int_1 = save.substring(0, boundLow);
    
    boundHigh = save.indexOf(delimiter, boundLow+1);
    int_2 = save.substring(boundLow+1, boundHigh);
    
    boundLow = save.indexOf(delimiter, boundHigh+1);
    int_3 = save.substring(boundHigh+1, boundLow);


if (int_1.length() > 1){

MHZ = int_1.toFloat();
if (int_2 == "1"){RxBW = 1;}if (int_2 == "2"){RxBW = 2;}if (int_2 == "3"){RxBW = 3;}if (int_2 == "4"){RxBW = 4;}
if (int_2 == "5"){RxBW = 5;}if (int_2 == "6"){RxBW = 6;}if (int_2 == "7"){RxBW = 7;}if (int_2 == "8"){RxBW = 8;}
if (int_2 == "9"){RxBW = 9;}if (int_2 == "10"){RxBW = 10;}if (int_2 == "11"){RxBW = 11;}if (int_2 == "12"){RxBW = 12;}
if (int_2 == "13"){RxBW = 13;}if (int_2 == "14"){RxBW = 14;}if (int_2 == "15"){RxBW = 15;}if (int_2 == "16"){RxBW = 16;}
ELECHOUSE_cc1101.setRxBW(RxBW); 
ELECHOUSE_cc1101.setMHZ(MHZ);
if (int_3 == "PA10"){ELECHOUSE_cc1101.Init(PA10);}  if (int_3 == "PA7"){ELECHOUSE_cc1101.Init(PA7);}  
if (int_3 == "PA5"){ELECHOUSE_cc1101.Init(PA5);} if (int_3 == "PA0"){ELECHOUSE_cc1101.Init(PA0);} 
if (int_3 == "PA_10"){ELECHOUSE_cc1101.Init(PA_10);}  if (int_3 == "PA_15"){ELECHOUSE_cc1101.Init(PA_15);} 
if (int_3 == "PA_20"){ELECHOUSE_cc1101.Init(PA_20);} if (int_3 == "PA30"){ELECHOUSE_cc1101.Init(PA_30);}
}else{
ELECHOUSE_cc1101.setRxBW(RxBW); 
ELECHOUSE_cc1101.setMHZ(MHZ);
ELECHOUSE_cc1101.Init(PA10);
}
save = "";
int_1 = "";
int_2 = "";
int_3 = "";

Serial.println("Saved to EEPROM");
cc1101 = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void esp_reconnect(){
input = "";
//Access Point
  Serial.println("Configuring access point...");
  WiFi.softAP(APssid, APpassword);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");

//Client
  Serial.println("Connecting to ");
  Serial.println(CLssid);
  WiFi.begin(CLssid, CLpassword);
  while (timeout < 30) {
    delay(500);
    Serial.print(".");
  if (WiFi.status() == WL_CONNECTED){
    timeout = 31;    
  }
   timeout = timeout + 1;
  }
  timeout = 0;
  if (WiFi.status() == WL_CONNECTED){
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("CL IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("CL Hostname: ");
  Serial.println(clhost);  
  server.begin();
}else{
  WiFi.disconnect();
  Serial.println("connection timeout!");
  Serial.println("WiFi not connected. AP Mode Only!");
}
show_wifi = 3;
Serial.println("Remote Fusion Ready");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void send_rcswitch() {
float mhz;
  if (repeater == 2){}else{
  mhz = int_2.toFloat();
  value = int_3.toInt();
  bits = int_4.toInt();
  prot = int_5.toInt();
  puls = int_6.toInt();
if(int_7.toInt() > 0){
  repRcs = int_7.toInt();
  }else{
  repRcs = 15;  
  }
  }
  input = "";

if (mhz >= 300 && mhz <= 348 || mhz >= 387 && mhz <= 464 || mhz >= 779 && mhz <= 928){
  ELECHOUSE_cc1101.SetTx(mhz);
}else{
  ELECHOUSE_cc1101.SetTx();  
}
  mySwitch.disableReceive();
  mySwitch.enableTransmit(pinTx);


  mySwitch.setRepeatTransmit(repRcs);
  mySwitch.setProtocol(prot);
  mySwitch.setPulseLength(puls);
  mySwitch.send(value, bits);

  mySwitch.disableTransmit();
  mySwitch.enableReceive(pinRx); 
  ELECHOUSE_cc1101.SetRx();
  
  Serial.println("RCS_Send_Finish");
  if (repeater == 2){repeater = 1; receive_all();}
    if (repeater == 1 || receiver == 1){value = 0; receive_all();}
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void send_newremoteswitch() {
float mhz;
if (repeater == 3){}else{
  mhz = int_2.toFloat();
  Address = int_3.toInt();
  Unit = int_4.toInt();
  GroupBit = int_5.toInt();
  SwitchType = int_6.toInt();
  Period = int_7.toInt();
  repNrs = int_8.toInt();
}
  
  input = "";
  
if (mhz >= 300 && mhz <= 348 || mhz >= 387 && mhz <= 464 || mhz >= 779 && mhz <= 928){
  ELECHOUSE_cc1101.SetTx(mhz);
}else{
  ELECHOUSE_cc1101.SetTx();  
}

  
  NewRemoteTransmitter transmitter(Address, pinTx, Period, repNrs);

if (SwitchType == 1){Switch = true;}else{Switch = false;}
if (GroupBit == 1){transmitter.sendGroup(Switch);}else{transmitter.sendUnit(Unit, Switch);}
  Serial.println("NRS_Send_Finish");
  if (repeater == 3){repeater = 1; receive_all();}
    if (repeater == 1 || receiver == 1){Address = 0; receive_all();}
 }
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dim_newremoteswitch() {
float mhz;
  mhz = int_2.toFloat();
  Address = int_3.toInt();
  Unit = int_4.toInt();
  DimLevel = int_5.toInt();
  Period = int_6.toInt();
  repNrs = int_7.toInt();

  
  input = "";
  
if (mhz >= 300 && mhz <= 348 || mhz >= 387 && mhz <= 464 || mhz >= 779 && mhz <= 928){
  ELECHOUSE_cc1101.SetTx(mhz);
}else{
  ELECHOUSE_cc1101.SetTx();  
}

  
  NewRemoteTransmitter transmitter(Address, pinTx, Period, repNrs);

  transmitter.sendDim(Unit, DimLevel); 
  Serial.println("NRS_Send_Finish");
  if (repeater == 1 || receiver == 1){Address = 0; receive_all();}
 }
  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void receive_rcswitch() {
float mhz;
if (int_2.toFloat() >= 300 && int_2.toFloat() <= 348 || int_2.toFloat() >= 387 && int_2.toFloat() <= 464 || int_2.toFloat() >= 779 && int_2.toFloat() <= 928){
  mhz = int_2.toFloat();
  ELECHOUSE_cc1101.SetRx(mhz); 
}else{
  ELECHOUSE_cc1101.SetRx();  
}
mySwitch.enableReceive(pinRx);
Serial.println("RCS Receiver initialized");

  do {
  WiFiClient client = server.available(); 
 
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read(); 
        Serial.write(c); 
        header += c;
        if (c == '\n') {  
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            boundLow = header.indexOf("HTTP");
            input = header.substring(5, boundLow);
            input.trim();
        

            client.println();
           break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;
}
}
}
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
   Serial.println("");
  }
   
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
    
    value = mySwitch.getReceivedValue();
    bits = mySwitch.getReceivedBitlength();
    prot = mySwitch.getReceivedProtocol();
    puls = mySwitch.getReceivedDelay();   

    if  (mySwitch.getReceivedValue() > 0){
    input = " ";  
    }
    
    mySwitch.resetAvailable();        
   }
    } 
        while( input.length() <= 0);
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void receive_newremoteswitch() {
float mhz;
if (int_2.toFloat() >= 300 && int_2.toFloat() <= 348 || int_2.toFloat() >= 387 && int_2.toFloat() <= 464 || int_2.toFloat() >= 779 && int_2.toFloat() <= 928){
  mhz = int_2.toFloat();
  ELECHOUSE_cc1101.SetRx(mhz);
}else{
  ELECHOUSE_cc1101.SetRx();  
}
  input = "";
  Serial.println("NRS Receiver initialized");  

  do {
  WiFiClient client = server.available();
 
  if (client) {      
    Serial.println("New Client."); 
    String currentLine = ""; 
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read(); 
        Serial.write(c);        
        header += c;
        if (c == '\n') {        
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            boundLow = header.indexOf("HTTP");
            input = header.substring(5, boundLow);
            input.trim();

            boundLow = input.indexOf(",");
            filter = input.substring(0, boundLow);
            filter.trim();
            Serial.println(input);
            client.println(filter);

            client.println();
           break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;
}
}
}

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
   Serial.println("");
  }

  NewRemoteReceiver::init(pinRx, 1, showCode);
  delay(500);
  if (Address > 0){
  input = " ";  
  }
    }
        while( input.length() <= 0);
        }
 
 void showCode(unsigned int period, unsigned long address, unsigned long groupBit, unsigned long unit, unsigned long switchType) {

  Serial.print(address);  
  Serial.print(",");  
  Serial.print(unit);
  Serial.print(",");  
  Serial.print(groupBit);
  Serial.print(",");  
  Serial.print(switchType);
  Serial.print(",");  
  Serial.print(period);
  Serial.println("");

  Address = address;
  Unit = unit;  
  GroupBit = groupBit;
  SwitchType = switchType;
  Period = period;          
  repNrs = 3;
  NewRemoteReceiver::disable();
  
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void receive_all() {
float mhz;
input = "";
Address = 0;
value = 0;
if (int_2.toFloat() >= 300 && int_2.toFloat() <= 348 || int_2.toFloat() >= 387 && int_2.toFloat() <= 464 || int_2.toFloat() >= 779 && int_2.toFloat() <= 928){
  mhz = int_2.toFloat();
  ELECHOUSE_cc1101.SetRx(mhz);
}else{
  ELECHOUSE_cc1101.SetRx(MHZ);  
}
Serial.println("ALL Receiver initialized");

  do {

  WiFiClient client = server.available();
 
  if (client) {                          
    Serial.println("New Client.");       
    String currentLine = "";             
    while (client.connected()) {         
      if (client.available()) {          
        char c = client.read();          
        Serial.write(c);                 
        header += c;
        if (c == '\n') {                 
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            
            boundLow = header.indexOf("HTTP");
            input = header.substring(5, boundLow);
            input.trim();

            boundLow = input.indexOf(",");
            filter = input.substring(0, boundLow);
            filter.trim();
            delay(200);
           
            if (filter.length() > 0){
            Serial.println(input);
            client.print(filter);
            }
if (fusion == 1){
   client.print(",");
   client.print(repeater);
   client.print(",");
   client.print(receiver);
   fusion = 0;
}
if (input == "fusion,0,0"){
  int_2 = "0";
  int_3 = "0";
  fusion_settings();
}


  if (repeater == 1 && Address > 0){
  repeater = 3;
            client.print("Repeat: nrs,");
            client.print(MHZ);
            client.print(",");
            client.print(Address);
            client.print(",");
            client.print(Unit);
            client.print(",");
            client.print(GroupBit);
            client.print(",");
            client.print(SwitchType);
            client.print(",");
            client.print(Period);
            client.print(",");
            client.println(repNrs);
            count = 0;
            input = " ";
  }  
  if (repeater == 1 && value > 0){
  repeater = 2;
            client.print("Repeat: rcs,");
            client.print(MHZ);
            client.print(",");
            client.print(value);
            client.print(",");
            client.print(bits);
            client.print(",");
            client.print(prot);
            client.print(",");
            client.print(puls);
            client.print(",");
            client.println(repRcs);
            count = 0;
            input = " ";
  }
  if (receiver == 1 && Address > 0){
            client.print("Receive: nrs,");
            client.print(MHZ);
            client.print(",");
            client.print(Address);
            client.print(",");
            client.print(Unit);
            client.print(",");
            client.print(GroupBit);
            client.print(",");
            client.print(SwitchType);
            client.print(",");
            client.println(Period);
            client.print("");
            client.println("");
            Address = 0;
  }
  
  if (receiver == 1 && value > 0){
            client.print("Receive: rcs,");
            client.print(MHZ);
            client.print(",");
            client.print(value);
            client.print(",");
            client.print(bits);
            client.print(",");
            client.print(prot);
            client.print(",");
            client.println(puls);
            value = 0;
  }

            
            client.println();
           break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;
}
}
}

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
   Serial.println("");
  }

if (value > 0 && count < 5 && repeater == 1){
  count = count +1; 
}
if (value > 0 && count > 4 && repeater == 1){
  repeater = 2;
  count = 0;
  input = " ";
}
if (Address > 0 && count < 5 && repeater == 1){
  count = count +1; 
}
if (Address > 0 && count > 4 && repeater == 1){
  repeater = 3;
  count = 0;
  input = " ";
}
  if (filter == "txrcs" || filter == "txnrs" || filter == "dimnrs"){

    boundLow = input.indexOf(delimiter);
    int_1 = input.substring(0, boundLow);
    
    boundHigh = input.indexOf(delimiter, boundLow+1);
    int_2 = input.substring(boundLow+1, boundHigh);
    
    boundLow = input.indexOf(delimiter, boundHigh+1);
    int_3 = input.substring(boundHigh+1, boundLow);
  
    boundHigh = input.indexOf(delimiter, boundLow+1);
    int_4 = input.substring(boundLow+1, boundHigh);

    boundLow = input.indexOf(delimiter, boundHigh+1);
    int_5 = input.substring(boundHigh+1, boundLow);

    boundHigh = input.indexOf(delimiter, boundLow+1);
    int_6 = input.substring(boundLow+1, boundHigh);

    boundLow = input.indexOf(delimiter, boundHigh+1);
    int_7 = input.substring(boundHigh+1, boundLow);
    
    int_8 = input.substring(boundLow+1);
    
    filter = "";
    
 if (int_1 == "txrcs"){
  send_rcswitch();
 }
  if (int_1 == "txnrs"){
  send_newremoteswitch();
}
  if (int_1 == "dimnrs"){
  dim_newremoteswitch();
} 
}

   mySwitch.enableReceive(pinRx);
   delay(200);
   if (mySwitch.getReceivedValue() > 0){

    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( mySwitch.getReceivedProtocol() );
    Serial.print(" Delay: ");    
    Serial.println( mySwitch.getReceivedDelay() );
    
    value = mySwitch.getReceivedValue();
    bits = mySwitch.getReceivedBitlength();
    prot = mySwitch.getReceivedProtocol();
    puls = mySwitch.getReceivedDelay();   
    mySwitch.resetAvailable();        
   }
   
  NewRemoteReceiver::init(pinRx, 1, showCode);
  delay(200);

if (repeater == 0 && receiver == 0){input = " ";}
  
    } 
        while( input.length() <= 0);
    }
