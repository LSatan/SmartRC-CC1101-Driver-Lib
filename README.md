# RCSwitch-CC1101-Driver-Lib_V2.21 HOTFIX ESP32!

---------------------------------------------
Donation
---------------------------------------------

If you like the library, support me with a donation.

"Some things are priceless - for everything else there is this donation fund"

"Coffee is a good partner - but it also has to be paid for"

https://www.paypal.me/LittleSatan666

Thank You!

---------------------------------------------
Remotes Fusion APP has the following features.
---------------------------------------------

- 1.Up to 15 transmitters with two buttons or one button.
- 2.RcSwitch and NewRemoteswitch mixed possible.
- 3.Each transmitter can broadcast on its own frequency.
- 4.Dimmer for NewRemoteSwitch. Can dim compatible lamps. 
- 5.Monitor mode. 
- 6.Repeater and receiver mode.
- 7.Transmitters can be taught in or manually set. 
- 8.Ap mode and client mode.
- 9.For access via the internet see the instructions.


---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V2.21
---------------------------------------------
20.01.2019

Driver Library		:Spi options have been removed. (Problems with ESP32) FIXED!

notes			:everything was checked again. D-SUN module on esp8266 / 32 and so on.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V2.2
---------------------------------------------
17.01.2019

Version notes		:Spi was reconfigured for fix some issuses.

Driver Library		:HotFix for cc1101 module with "8pins"(example DSun). Transmitting on Esp8266 based bords not work. FIXED!

Driver Library		:Change "SpiReadStatus" to public. Now can read RSSI Level!

Driver Library		:Spi transfer rate was limited to cc1101 max transfer rate (10mhz).

Driver Library		:After using Spi, it will now be disabled. (receive data from serial monitor and rx pin was not working on Esp 8266/32) FIXED!

Add Examples		:ReceiveDemo_Simple_with_RSSI_cc1101.ino (Shows reception strength)

Add Examples		:Frequency_Scanner_cc1101.ino (Scans Transmitting frequency of your remotes. No good results with cc1101-DSun)

Add Examples		:ProtocolAnalyzeDemo_cc1101.ino (for scan unknown protokolls. Requires: link is in sketch!) 

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V2.1
---------------------------------------------
3.1.2019

Driver Library		:Spi corrections for esp32. Spi Pin changes did not work on esp32. FIXED!

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V2.0
---------------------------------------------
18.12.2018

Driver Library		:Own Spi pin settings possible. Allows the use of other microcontrollers. Allows you to use multiple CC1101s. More information in the commands.txt

Add TXT			:driver lib commands and examples.txt

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.93
---------------------------------------------
27.11.2018

Driver Library		:setESP8266 has been removed. Pins are set automatically with the init command.

Driver Library		:Arduino mega SPI pins added.

Driver Library		:Added setGDO pins (For libraries that directly address the GDO pins).Like this https://github.com/EinfachArne/Somfy_Remote .
			 Example ELECHOUSE_CC1101.setGDO(2, 4); // (pin2 = GDO0, pin4 = GDO2).

Add Wiring TXT		:WIRING MEGA.txt

Add Wiring JPG		:WIRING MEGA.jpg

LICENSE			:LICENSE.txt Update.

notes			:setGDO is not compatible with RC_Switch / NewRemotSwitch.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.92
---------------------------------------------
16.11.2018

Fixes Remotes Fusion	:Receive mode and Repeater mode frequency, changes to the transmitter frequency after pressing on a transmitter. (RemotesFusion.ino)Fixed!

NewRemoteSwitch		:LearnCode_cc1101.ino (corrections).

Added			:Keywords.txt added.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.91
---------------------------------------------
07.11.2018

New Sketch		:Clear_EEPROM.ino (if there are problems with the access point password or if you want to use your ESP for other projects).

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.9
---------------------------------------------
18.10.2018

New Sketch		:RemotesFusion.ino.

New Apk´s		:RemotesFusion_V1_en.apk / RemotesFusion_V1_de.apk.

Add Docs		:Docs and manuals for RemotesFusion.

Changes			:channel spacing and channel set was removed from the examples. unnecessary because exact frequency can be set.

notes			:RemotesFusion is an application with which Rc-Switch and NewRemoteSwitch can be controlled at the same time.
			 A universal remote control APP with many possibilities. Only for Esp 8266/32 modules! For more information, watch the docs.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.8
---------------------------------------------
[Download Release V1.8](https://drive.google.com/file/d/1ADKOmW0SrUoomeoDskFrm0J2SdwCLB_O/view?usp=sharing)

02.09.2018

Driver Library		:Added pin settings for esp32.

Examples		:Added pin settings for esp32 examples.

Add Wiring JPG		:Add Wiring JPG esp32.

Add Wiring TXT		:Add Wiring TXT esp32.

Add Wiring JPG		:Add Wiring JPG esp32 for Receive_Send_Decimal_Demo_Simple.ino.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.72
---------------------------------------------
[Download Release V1.72](https://drive.google.com/file/d/1sfMvQw2JaGARTaYXZevOjB20BFOMS0tw/view?usp=sharing)

16.08.2018

Examples		:All NewRemoteSwitch examples are compatible with ESP. Exception Retransmitter_cc1101.ino.

Driver Library		:Frequency calculator calibrated. Difference -0.01 MHz. (Fixed!) Frequency is now accurate.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.71
---------------------------------------------
[Download Release V1.71](https://drive.google.com/file/d/1PXa1k0AIDY8bTMxFyyQMbOljijhIn1ke/view?usp=sharing)

10.08.2018

Driver Library		:Now you can Switch the frequency in loop. Examples: ELECHOUSE_cc1101.SetTx(433.92); and ELECHOUSE_cc1101.SetRx(433.92);.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.7
---------------------------------------------
[Download Release V1.7](https://drive.google.com/file/d/1uxACfe1ZUrkL4S3_NPVN0DmUfzbxt6oQ/view?usp=sharing)

09.08.2018

Driver Library		:Now you can enter your base frequency directly.The library calculates the settings automatically. example(433.92) 

Driver Library		:Remove freq2, freq1 and freq0 Settings.

Driver Library		:Removed unnecessary entries. the library shrank from 32,8kb(.cpp) and 9,38kb(.h) to 17,0kb(.cpp) and 7,87kb(.h).

New Example		:(NewRemoteSwitch Library) NewRemoteRepeater.ino. Repeat the Received signal 1to1. Best thanks to Roman for write it. 

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.61
---------------------------------------------
[Download Release V1.61](https://drive.google.com/file/d/1q8FV5kDnhAj1SMZf6DS0wDcm5xo4E-5-/view?usp=sharing)

05.08.2018

Examples Fix (rc-switch):Fix auto receive pulse. Received pulse don´t transmit. Fixed!(Repeater.ino and Receive_Send_Decimal_Demo_Simple.ino).

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.6
---------------------------------------------
[Download Release V1.6](https://drive.google.com/file/d/1p_iAxh7ZWlNWhFoqMM_tlLTLdyGK07-a/view?usp=sharing)

03.08.2018

Esp8266			:Compatibility for all RC switch examples.

Driver Library		:Add the GDO2 pin for Receive. Change GDO0 to send.

Wiring changes		:Description in jpg's and txt's.

Examples		:All examples have been adjusted.

Examples		:Esp and Arduino are set automatically. 

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.5
---------------------------------------------
[Download Release V1.5](https://drive.google.com/file/d/1XcGPbvI5v2PcpnVBbjtOFwtuJtx3jMK4/view?usp=sharing)

31.07.2018

New			:Supportet NewRemoteSwitch Library. For wireless sockets by trust smart home, Smappee, DI-O Chacon, Intertechno and others.
			 You want to use it? Then you need that https://github.com/1technophile/NewRemoteSwitch
			 
Add Examples		:NewRemoteSwitch Compatible examples.

Driver Library		:You can now switch directly from Tx to Rx in the loop. Without resetting the cc1101.

Add Examples		:New Repeater Example for Rc-switch. For example, to increase the range of your remote control.

Example changes		:Receive_Send_Decimal_Demo_Simple.ino (remove unnecessary commands).

Examples Fix		:Rc-Switch examples fix TypeA, TypeB and TypeD (missing from semicolon to SetTx).

Notes			:NewRemoteSwitch examples Retransmitter_cc1101.ino and LightShow_cc1101.ino only Arduino!!!

Notes			:Rc-switch example Repeater_cc1101.ino only Arduino!!!

Notes			:Rc-switch example Receive_Send_Decimal_Demo_Simple.ino only Arduino!!!

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.4
---------------------------------------------
[Download Release V1.4](https://drive.google.com/file/d/1nAPGeQSutfskirJsng44adB4sXU1YU6A/view?usp=sharing)

26.07.2018

Driver Library		:esp8266 Compatibility (Tested with Wemos d1 mini).

Driver Library		:Change default Receive filter bandwidth to 812khz (max).

Driver Library		:Change default Tx Power to PA10 (max).

Examples		:Add set esp command and pin settings.

Examples		:Diskription change.

Demo Example		:Add auto Recive and Send pulse length. (Receive_Send_Decimal_Demo_Simple.ino)

Add Wiring JPG		:Add Wiring JPG esp8266.

Add Wiring TXT		:Add Wiring TXT esp8266.

Notes			:esp8266 pin D4 don´t work with receive, Transmit and receive is set to D2.

Notes 			:esp8266 don´t work with Receive_Send_Decimal_Demo_Simple.ino (freezes when reset cc1101).

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.31
---------------------------------------------
[Download Release V1.31](https://drive.google.com/file/d/1iKmagldd14O1Boa9Z_PDpXbHRECYUPdt/view?usp=sharing)

30.03.2018:

Driver Library Fix	:no effect set by transmission power.

Driver Library		:Transmitting power can be adjusted via frequency(example in the sketches).

Examples		:Command Tx Power cleared.

Examples Fix		:set Receive filter bandwidth (command incomplete) Fixed.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.3
---------------------------------------------
[Download Release V1.3](https://drive.google.com/file/d/1q56Qewk8-Aquv1epss1gd7Gc05q4GrbO/view?usp=sharing)

25.03.2018:

Driver Library		:Add Channel List. Now you can set channel numbers 0-255.

Driver Library		:Add bandwidth can be adjusted

Driver Library		:transmission power can be adjusted

Driver Library		:can set your own frequency settings (freq2, freq1, freq0)

Driver Library		:Channel spacing can be set.

Examples		:Add set new commands

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.2 
---------------------------------------------
[Download Release V1.2](https://drive.google.com/file/d/1KLJt8ygszj9rqcttiEMt5zCnJKHc_qe3/view?usp=sharing)

20.03.2018:

Driver Library 		:Add 315Mhz Support.

Driver Library 		:Add Channel command for finetune (80khz steps) from 420mhz - 440mhz / 302mhz - 322mhz.

Examples		:Add set channel command in examples.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.1 
---------------------------------------------
[Download Release V1.1](https://drive.google.com/file/d/1uZzcY4uoduiUjFZzXdxA-ucLoA_TtC1f/view?usp=sharing)

19.03.2018:

Driver Library Fix	:Calibrated Frequency from 433.86 to 433.92 - 433.93 Mhz.

Driver Library		:cc1101 reset command added (allows switching between send and receive.)

New Demo Example	:Receive_Send_Decimal_Demo_Simple.ino (Sends last received decimal code)
			 To illustrate the change between RX and TX (press button = send / do not press button = receive)

Add Wiring JPG		:Wiring for Receive_Send_Decimal_Demo_Simple.ino

Add Wiring JPG		:Wiring for Nano / Uno


---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.0 
---------------------------------------------
[Download Release V1.0](https://drive.google.com/file/d/14538RtiEakZ_8yioXJT32XneheSjDqxi/view?usp=sharing)

18.03.2018:

cc1101 Compatibility for RC-Switch.

cc1101 RC-Switch Compatible examples.