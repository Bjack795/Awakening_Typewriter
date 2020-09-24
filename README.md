# Awakening_Typewriter
E-ink Typewriter, Wemos Lolin D32 pro v2, Waveshare ePaper 2.9" screen and USB host for the keyboard. All connected by a designed PCB.

## Hardware

- Wemos Lolin D32 pro v2.0 (https://wiki.wemos.cc/products:d32:d32_pro)
- 296x128, 2.9inch E-Ink display module (https://www.waveshare.com/product/2.9inch-e-paper-module.htm)
- Keyboard USB Host (http://www.hobbytronics.co.uk/usb-host-keyboard)
- microSD
- TP4056
- Lipo battery 3.7 V, 1100 mAh
- mechanical keyboard
- designed PCB

The schematics of the connections are in the EPD_TYPEWRITER_Connections.pdf file
![EPAPER_TYPEWRITER_Connections](https://user-images.githubusercontent.com/42472256/94129163-52a14080-fe5b-11ea-9a0e-92155a6a69cb.png)

## Software
All the libraries and files included in TYPEWRITER_EPD folder and downloaded from the library manager.

Arduino IDE must be used to upload all these files.

**REMEMBER**

Add "esp32" in Board Manager of Arduino IDE or follow this https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/windows.md procedure to have the latest commits (this should fix some bugs).
Then select Lolin D32 pro from the boards list.

### EPD library Rights

All the EPD functions rights belong to ZinggJM (https://github.com/ZinggJM), a genius whose patience and knowledge has been of incredible help in bringing this project up to light.

The functions are an implementation of this library https://github.com/ZinggJM/GxEPD2 and of Adafruit GFX library https://github.com/adafruit/Adafruit-GFX-Library.

### WebServer Rights

All the WebServer functions rights belong to G6EJD (https://github.com/G6EJD), a genius to whom goes my gratitude for the huge help given to this project, follow him here https://www.youtube.com/user/G6EJD, it's worth it.

The functions are an implementation of this library https://github.com/tzapu/WiFiManager/tree/development and of the standard Webserver.h built-in library.

## Awakening Typewriter 2.0 slim (AWT 2.0 slim)
This is the actual aspect of the project.

![1](https://user-images.githubusercontent.com/42472256/62328355-39993b80-b4b3-11e9-98f6-0a0668b5841e.jpg)
![2](https://user-images.githubusercontent.com/42472256/62328374-47e75780-b4b3-11e9-8be5-450779a3f3ca.jpg)
![3](https://user-images.githubusercontent.com/42472256/62328389-503f9280-b4b3-11e9-84e6-70d9a5628e3f.jpg)

