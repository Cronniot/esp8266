# Cronniot esp8266
[Cronniot](https://cronniot.com) is a new Internet of Things platform designed to combine an ease use with advanced technical features. Its easy operation allows technology enthusiasts to program their own devices and connect them to the Internet.

For that we have made available this project, which contains the source code to program ESP8266 controllers and connect them to the Cronniot cloud service.

## The esp8266 Controller

ESP8266 is a low-cost chip with which manufacturers are making wirelessly networkable modules and making that the Internet of things is really starting to take off.

This project includes the modules that support the ESP8266 controller, you can see a complete list of all the modules available in this [link](http://www.esp8266.com/wiki/doku.php?id=esp8266-module-family) and you can buy them in any online store.

## Configure Arduino IDE

Arduino IDE is required to program, compile and upload the ESP8266 driver. Install the current upstream Arduino IDE at the 1.8 level or later. The current version is at the [Arduino website](http://www.arduino.cc/en/main/software).

Start Arduino and open Preferences window. In Settings tab enter ```http://arduino.esp8266.com/stable/package_esp8266com_index.json``` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.

Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

## Configure a new device

To start adding a new device download or clone this repository into your local environment and open it from your Arduino IDE. Go to the `"Information to fill"` line and place the parameters of your device:

- Wifi name
- Wifi password
- Controller Id (From cronniot device parameters)
- Secret key (From cronniot device parameters)
- Device Id (From cronniot device parameters)

> You need to add a new device, if you have not already done, from the online [cronniot](https://cronniot.com/webapp) platform and copy your device parameters.

Note that you have an array of digital pins, where each pin is a device output. Therefore you must assign a device (device Id) to a pin of the controller. In the same way you can have many devices in the same controller.
```c++
const pinDevice digitPins[] { {"mydeviceid", 9}, {"otherdeviceid", 10} };
```
<p align="center">
  <img src="https://cronniot.com/img/nodemcu_pinout.png">
</p>

Once you set the parameters you can compile and upload the firmware to the device. Then you must pair the device with the cronniot service to start using it.