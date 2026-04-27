# SmartRC CC1101 Driver Library (V3.0.1)

![Version](https://img.shields.io/badge/Version-3.0.0-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Arduino%20%7C%20ESP8266%20%7C%20ESP32-green.svg)
![License](https://img.shields.io/badge/License-MIT-orange.svg)

This is the official evolution of the **SmartRC CC1101** library. After years of community use (Versions up to 2.5.7), the entire driver core has been completely re-engineered for **2026** to ensure maximum precision, stability, and minimal resource footprint.

## 💎 The Evolution: Legacy vs. V3.0.0

While the original versions relied on experimental values and global variables, V3.0.0 strictly follows the physical formulas provided in the Texas Instruments CC1101 datasheet and features a modern, object-oriented design.

### Key Technical Improvements:

* **Read-Modify-Write (RMW) Logic:** Instead of mirroring chip states in the microcontroller's RAM, the driver now operates bit-precisely directly on the hardware registers. This saves significant RAM on systems like the Arduino Nano/Uno.

* **Mathematical Precision:** Frequencies, data rates, and bandwidths are now calculated using exact logarithmic formulas, drastically increasing link stability.

* **Object-Oriented Architecture:** Full support for multi-module setups (e.g., Dual-Band Gateways 433/868 MHz) via class instantiation. Old sketches remain 100% compatible via the provided wrapper!

* **Hardware Feedback:** New "Plain-Text" Getters (`getMHZ()`, `getDRate()`, etc.) allow you to verify the actual internal settings of the chip for the first time.

## ⚠️ Backwards Compatibility & Removed Functions

We kept the transition as smooth as possible. **Most legacy sketches using `ELECHOUSE_CC1101` will compile and run out-of-the-box** thanks to a built-in wrapper!

However, to keep the library clean and professional, we removed the outdated multi-module workaround functions:
* ❌ `setModul()`, `addSpiPin()`, `addGDO()`, `addGDO0()` have been **removed**.
* ✅ **New Approach:** If you want to use multiple CC1101 modules, simply instantiate multiple objects (e.g., `SmartRC_CC1101 radio1;` and `SmartRC_CC1101 radio2;`).

## 🛠 Issue Policy & 2026 Reboot

With the release of **Version 3.0.0** in April 2026, the library architecture has been fundamentally changed.

**What does this mean for open issues?**

* **Legacy Issues:** All issues reported before April 2026 refer to the outdated V2.x architecture. As these versions are no longer actively maintained, these issues have been closed and moved to the **"Legacy V1.x/V2.x"** milestone.

* **A Fresh Start:** If you encounter a bug with the new version, please open a **new issue**.

* **Prerequisite:** Ensure you are using the new object-oriented structure and that your problem is reproducible with the current V3.0.0 release.

## 🔌 Hardware Wiring

| **CC1101 Pin** | **Arduino Uno/Nano** | **ESP32 (Default)** | **Description** |
| ----- | ----- | ----- | ----- |
| **VCC** | 3.3V | 3.3V | Power Supply (3.3V ONLY!) |
| **GND** | GND | GND | Ground |
| **SCK** | Pin 13 | GPIO 18 | SPI Clock |
| **MISO** | Pin 12 | GPIO 19 | SPI Data Out |
| **MOSI** | Pin 11 | GPIO 23 | SPI Data In |
| **CSN / SS** | Pin 10 | GPIO 5 | Chip Select |
| **GDO0** | Pin 2 | GPIO 2 | Interrupt/Data Out |

## 🚀 Quick Start

```cpp
#include <SPI.h>
#include "SmartRC_CC1101.h"

// Create an instance
SmartRC_CC1101 radio;

void setup() {
    Serial.begin(115200);
    radio.Init();
    radio.setMHZ(433.92);
    radio.SetRx();
    
    if(radio.getCC1101()) {
        Serial.println("Hardware found and ready!");
    }
}

void loop() {
    if (radio.CheckReceiveFlag()) {
        byte buffer[64];
        byte len = radio.ReceiveData(buffer);
        // Process data...
    }
}
```

## 🔬 Hardware Calibration (New in V3.0.0)

A common issue with inexpensive CC1101 modules is a severe deviation of the integrated 26 MHz crystal oscillator (often ±20 ppm to ±30 ppm). This causes the module to transmit slightly off-center (e.g., at 433.911 MHz instead of 433.920 MHz), leading to massive range loss with narrow-band receivers.

Check the `examples/` folder for our new **Calibration Tool**. With the help of an SDR (Software Defined Radio) and the Serial Monitor, you can completely eliminate this hardware flaw:

* **Band Calibration Tool:** A guided script to measure the upper and lower limits of an entire frequency band (e.g., the 433 MHz band). It calculates the perfect `FSCTRL0` register offsets and outputs ready-to-use C++ code (`myRadio.setClb(...)`) to permanently tune your specific hardware module!

## 🙏 Acknowledgments & History

This library was originally based on the Elechouse CC1101 Library by **Wilson Shen**. A sincere thank you goes to Wilson and the Elechouse team for their kind permission to modify and evolve this code. Their original work made this community-standard driver possible.

Visit the original source at: [www.elechouse.com](http://www.elechouse.com)

## 📜 License

Released under the MIT License.
**Author:** Little Satan (SmartRC)
**Revision:** April 2026
