# One Shot Blinking Light Firmware

Arduino-based firmware developed in the PlatformIO development environment.

## Firmware Notes
Fireware is written to work with the Arduino Nano Every, but can be adapted to
the Nano 33 BLE.
## Circuit
KiCad project is under `circuit/`.

![schematic.png](circuit/schematic.png)

## Troubleshooting
```
avrdude: ser_open(): can't open device "/dev/ttyACM0": Permission denied
```

Make sure that you set permissions to allow your device connected via USB to be
accessed via VS Code:
https://support.arduino.cc/hc/en-us/articles/360016495679-Fix-port-access-on-Linux

## Resources
* https://docs.arduino.cc/resources/datasheets/ABX00028-datasheet.pdf
* https://docs.arduino.cc/resources/datasheets/ABX00031-datasheet.pdf
* https://docs.platformio.org/en/stable/boards/atmelmegaavr/nano_every.html
* https://docs.platformio.org/en/latest/boards/nordicnrf52/nano33ble.html

Copyright (c) [2023] [Mark L. Palmeri]
