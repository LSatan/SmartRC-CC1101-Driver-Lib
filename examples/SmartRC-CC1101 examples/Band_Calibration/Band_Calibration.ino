/*
* CC1101 Band Calibration Tool
* -----------------------------------------------------------
* With this tool, you determine the exact offset values (clb)
* for the FSCTRL0 register for a specific frequency band.
* * The lower and upper limit frequencies of the band are
* sent each time. You measure the actual frequency with an SDR
* and enter it here. The program calculates from this the
* perfect correction values for setClb().
*/

#include <SmartRC_CC1101.h>

SmartRC_CC1101 myRadio;

// States for the menu navigation
enum State { SELECT_BAND, MEASURE_LOW, MEASURE_HIGH };
State currentState = SELECT_BAND;

int selectedBand = 0;
float freqLow = 0.0;
float freqHigh = 0.0;
int clbLow = 0;
int clbHigh = 0;

unsigned long reset = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // Configure CC1101 pins (Adjust if necessary!)
  // myRadio.setSpiPin(14, 12, 13, 27); //(SCK, MISO, MOSI, SS/CSN)
  myRadio.setGDO0(2);
  
  myRadio.Init();

  if (!myRadio.getCC1101()) {
    Serial.println("ERROR: CC1101 not found!");
    while (1);
  }

  // Send a clear continuous tone (ASK/OOK) for the measurement
  myRadio.setCCMode(1);
  myRadio.setModulation(2);
  myRadio.setPA(10); 

  printMainMenu();
}

void loop() {
  // Send a LONG test packet every 1000ms so that the signal becomes massively visible in the SDR
  if (currentState == MEASURE_LOW || currentState == MEASURE_HIGH) {
    if (millis() - reset < 1000) {
      byte testPacket[60]; 
      memset(testPacket, 0xAA, 60); // Fills the 60-byte array completely with 0xAA
      myRadio.SendData(testPacket, 60); // Sends the extra-long package
    }else if (millis() - reset > 2000){
      reset = millis();
    }
  }

  // Process user inputs
  if (Serial.available() > 0) {
    // 1. Safely read text up to the end of the line (or timeout)
    String inputStr = Serial.readStringUntil('\n');
   
    // 2. Remove invisible spaces and control characters (\r, \n)
    inputStr.trim(); 
    
    // 3. Ignore empty inputs (e.g., only Enter pressed)
    if (inputStr.length() == 0) return; 

    // 4. Convert the clean text into a decimal number
    float input = inputStr.toFloat();
    
    if (input == 0.0) {
      Serial.println("Invalid input. Please enter a valid number.");
      return; 
    }

    switch (currentState) {
      case SELECT_BAND:
        handleBandSelection((int)input);
        break;
        
      case MEASURE_LOW:
        clbLow = calculateFSCTRL0(freqLow, input);
        Serial.print("-> Calculated Offset (Low): ");
        Serial.println(clbLow);
        Serial.println("-------------------------------------------");
        
        // Switch to the higher frequency
        myRadio.setMHZ(freqHigh);
        Serial.print("Measure now at the UPPER end at: ");
        Serial.print(freqHigh, 3);
        Serial.println(" MHz.");
        Serial.println("Enter the precisely measured frequency in the SDR:");
        currentState = MEASURE_HIGH;
        break;

      case MEASURE_HIGH:
        clbHigh = calculateFSCTRL0(freqHigh, input);
        Serial.print("-> Calculated Offset (High): ");
        Serial.println(clbHigh);
        Serial.println("-------------------------------------------");
        
        // Output result
        printResult();
        printMainMenu();
        currentState = SELECT_BAND;
        break;
    }
  }
}

// Calculates the necessary FSCTRL0 value based on the deviation
int calculateFSCTRL0(float targetFreq, float actualFreq) {
  // Read current FSCTRL0 value (as signed int8_t!)
  int8_t currentOffset = (int8_t)myRadio.SpiReadReg(0x0C); // 0x0C is FSCTRL0
  
  // Frequency deviation in MHz
  float deltaFreq = targetFreq - actualFreq;
  
  // The resolution of the FSCTRL0 register is F_OSC / 2^14
  // At 26 MHz: 26.0 / 16384 = 0.001586914 MHz (approx. 1.59 kHz) per tick
  float stepSize = 26.0 / 16384.0;
  
  // How many ticks do we need to shift the offset?
  int offsetTicks = round(deltaFreq / stepSize);
  
  // Calculate new value and limit to 8-bit signed (-128 to +127)
  int newOffset = currentOffset + offsetTicks;
  if (newOffset > 127) newOffset = 127;
  if (newOffset < -128) newOffset = -128;
  
  // Return the offset as an unsigned byte for the register (2's complement is preserved)
  return (byte)newOffset;
}

void handleBandSelection(int band) {
  selectedBand = band;
  switch (band) {
    case 1: freqLow = 300.0; freqHigh = 348.0; break;
    case 2: freqLow = 378.0; freqHigh = 464.0; break;
    case 3: freqLow = 779.0; freqHigh = 899.0; break;
    case 4: freqLow = 900.0; freqHigh = 928.0; break;
    default:
      Serial.println("Invalid frequency band! Please enter 1, 2, 3, or 4.");
      return;
  }
  
  Serial.print("\nStart calibration for band");
  Serial.println(selectedBand);
  
  myRadio.setMHZ(freqLow);
  
  Serial.println("-------------------------------------------");
  Serial.print("Measure now at the LOWER end at: ");
  Serial.print(freqLow, 3);
  Serial.println(" MHz.");
  Serial.println("Set the SDR to the frequency and search for the signal.");
  Serial.println("Enter the precisely measured frequency in MHz (e.g., 378.015):");
  
  currentState = MEASURE_LOW;
}

void printMainMenu() {
  Serial.println("\n===========================================");
  Serial.println("CC1101 BAND CALIBRATION TOOL V2.0");
  Serial.println("===========================================");
  Serial.println("Select the frequency band to be calibrated:");
  Serial.println(" 1 : 315 MHz Band (300 - 348 MHz)");
  Serial.println(" 2 : 433 MHz Band (378 - 464 MHz)");
  Serial.println(" 3 : 868 MHz Band (779 - 899 MHz)");
  Serial.println(" 4 : 915 MHz Band (900 - 928 MHz)");
  Serial.println("-> Enter 1, 2, 3, or 4 and press Enter:");
}

void printResult() {
  Serial.println("\n===========================================");
  Serial.println("CALIBRATION SUCCESSFUL!");
  Serial.println("===========================================");
  Serial.println("Copy the following line into the setup() function");
  Serial.println("of your future projects (BEFORE myRadio.Init()):");
  Serial.println("");
  
  Serial.print("  myRadio.setClb(");
  Serial.print(selectedBand);
  Serial.print(", ");
  Serial.print(clbLow);
  Serial.print(", ");
  Serial.print(clbHigh);
  Serial.println(");");
  Serial.println("");
  Serial.println("With that, this module is perfectly adjusted for this band!");
  Serial.println("===========================================\n");
  delay(3000);
}