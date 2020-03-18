// These examples are from the Electronics Cookbook by Simon Monk
//https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
// mod by Little_S@tan

#include <ELECHOUSE_CC1101_SRC_DRV.h>

byte transmitt_byte[11] = {72,101,108,108,111,32,87,111,114,108,100};
char *transmitt_char = "Hello World";

void setup() {
    Serial.begin(9600);
    Serial.println("Tx");
    ELECHOUSE_cc1101.setGDO(6,2);        // set lib internal gdo pins (gdo0,gdo2).
    ELECHOUSE_cc1101.setCCMode(1);      // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0); // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);  // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
 // ELECHOUSE_cc1101.setPA(10);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
    ELECHOUSE_cc1101.Init();        // must be set to initialize the cc1101!
}

void loop() {


//Transmitt "Hello World" from byte format.
ELECHOUSE_cc1101.SendData(transmitt_byte, 11);
delay(2000);

//Transmitt "Hello World" from char format.
int len = strlen(transmitt_char);
ELECHOUSE_cc1101.SendData(transmitt_char, len);
delay(2000);
}
