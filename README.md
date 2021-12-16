# Teams PTT BTTN - The BLE push-to-talk button for Microsoft Teams
[![License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](http://doge.mit-license.org)

## Using the device
- Flash the latest firmware to your device. Power it up.
- The device acts now as a bluetooth keyboard with just one button. The device advertises itself as "Teams PTT BTTN", manufacturer is "TB Devices", battery ist always 100 % :-).
- Connect your device running Microsoft teams with this keyboard. 
- With the meeting window focused and muted, press the button of your device to talk.
- How does it work? The device just sends the key combination "Ctrl + Space" when you press it's button and releases it when you release the button.

## Builds for different platforms
### M5 Atom (Lite)
- Use build "m5stack-atom". 
- Button and LED is on board, no need to connect anything external.

### M5 Stamp PICO
- Use build "m5stack-stamp". 
- Connect button between G19 and GND
- Connect LED between G25 and GND using a 200R resistor
- Power with 5V and GND

### Other platforms
The should work on every ESP32 just add your own board definition to "platformio.ini"


## Update firmware
- Press button five times in fast sequence.
- Device spins up it's own WiFi access point with the name "Teams PTT BTTN". Connect to it using password "12345678".
- Onboard RGB LED (if the board has one) lights up blue.
- When connected, open the URL http://192.168.4.1
- Upload the new firmware .bin file. Device reboots. Done.

## Libraries used
This project uses the following libraries from different authors:
- [EasyButton](https://github.com/evert-arias/EasyButton) by evert-arias
- [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) by T-vK
- [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) by h2zero
- [SmartLeds](https://github.com/roboticsbrno/SmartLeds) by roboticsbrno


Thanks to all the authors.

## Licence
All code is licensed under the [MPLv2 License](https://github.com/toblum/ESPTeamsPresence/blob/master/LICENSE).