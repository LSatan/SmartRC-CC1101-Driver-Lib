#include <SmartRC_CC1101.h>

// Create two independent objects for your two modules
SmartRC_CC1101 modul1;
SmartRC_CC1101 modul2;

// --- Pin assignment for Arduino Uno / Nano ---
// Shared SPI pins: SCK = 13, MISO = 12, MOSI = 11

// Unique pins for Module 1
const byte SS_MODUL_1   = 10;
const byte GDO0_MODUL_1 = 2;

// Unique pins for Module 2
const byte SS_MODUL_2   = 9;
const byte GDO0_MODUL_2 = 3;

void setup() {
  Serial.begin(9600);

  // --- Configuration Module 1 ---
  // SCK, MISO, MOSI, SS
  modul1.setSpiPin(13, 12, 11, SS_MODUL_1); 
  // GDO0
  modul1.setGDO0(GDO0_MODUL_1);           
  
  modul1.Init();
  if (!modul1.getCC1101()) {
    Serial.println("ERROR: Modul 1 not found!");
    while (1);
  }
  modul1.setCCMode(1);
  modul1.setModulation(2); // ASK/OOK
  modul1.setMHZ(433.92);
  Serial.println("Modul 1 initialisiert auf 433.92 MHz.");


  // --- Configuration Modul 2 ---
  modul2.setSpiPin(13, 12, 11, SS_MODUL_2);
  modul2.setGDO0(GDO0_MODUL_2);

  modul2.Init();
  if (!modul2.getCC1101()) {
    Serial.println("ERROR: Modul 2 not found!");
    while (1);
  }
  modul2.setCCMode(1);
  modul2.setModulation(2);
  modul2.setMHZ(868.3); //Example: Module 2 is running at a different frequency!
  Serial.println("Modul 2 initialisiert auf 868.3 MHz.");
}

void loop() {
  // Example: Module 1 sends something
  char text1[] = "Hello from Module 1";
  modul1.SendData(text1);
  Serial.println("Module 1 has sent.");
  delay(1000);

  // Example: Module 2 sends something
  char text2[] = "Hello from Module 2";
  modul2.SendData(text2);
  Serial.println("Module 2 has sent.");
  delay(1000);

  /*
  * If you want to receive:
  * module1.CheckReceiveFlag()
  * module2.CheckReceiveFlag()
   */
}