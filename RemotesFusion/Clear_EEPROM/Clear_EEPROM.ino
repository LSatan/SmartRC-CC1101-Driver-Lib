/*Clears the EEPROM. 
For problems with password settings 
or to use the ESP in other projects.
*/

#include <EEPROM.h>


void setup() {
Serial.begin(115200);
Serial.println("EEPROM Cleaner ready!");
Serial.print("Start in .");
for (int i = 5; i>0; i--){
Serial.print(i);
Serial.print(".");
delay(1000);
}
EEPROM.begin(512);
for (int i=0; i< 512; i++){
EEPROM.write(i, 255);
Serial.print(".");
}
EEPROM.commit();
Serial.println("");
Serial.println("EEPROM Cleared!");
}
void loop(){
}
