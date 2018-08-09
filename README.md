# RCSwitch-CC1101-Driver-Lib_V1.61

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.7
---------------------------------------------
09.08.2018

Driver Library		:Now you can enter your base frequency directly.The library calculates the settings automatically. example(433.92) 

Driver Library		:Remove freq2, freq1 and freq0 Settings.

Driver Library		:Removed unnecessary entries. the library shrank from 32,8kb(.cpp) and 9,38kb(.h) to 17,0kb(.cpp) and 7,87kb(.h).

New Example		:(NewRemoteSwitch Library) NewRemoteRepeater.ino. Repeat the Received signal 1to1. Best thanks to Roman for write it. 

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.61
---------------------------------------------
05.08.2018

Examples Fix (rc-switch):Fix auto receive pulse. Received pulse don´t transmit. Fixed!(Repeater.ino and Receive_Send_Decimal_Demo_Simple.ino).

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.6
---------------------------------------------
03.08.2018

Esp8266			:Compatibility for all RC switch examples.

Driver Library		:Add the GDO2 pin for Receive. Change GDO0 to send.

Wiring changes		:Description in jpg's and txt's.

Examples		:All examples have been adjusted.

Examples		:Esp and Arduino are set automatically. 

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.5
---------------------------------------------
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
30.03.2018:

Driver Library Fix	:no effect set by transmission power.

Driver Library		:Transmitting power can be adjusted via frequency(example in the sketches).

Examples		:Command Tx Power cleared.

Examples Fix		:set Receive filter bandwidth (command incomplete) Fixed.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.3
---------------------------------------------
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
20.03.2018:

Driver Library 		:Add 315Mhz Support.

Driver Library 		:Add Channel command for finetune (80khz steps) from 420mhz - 440mhz / 302mhz - 322mhz.

Examples		:Add set channel command in examples.

---------------------------------------------
Changelog: RCSwitch-CC1101-Driver-Lib_V1.1 
---------------------------------------------
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
18.03.2018:

cc1101 Compatibility for RC-Switch.

cc1101 RC-Switch Compatible examples.