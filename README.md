# One Shot Blinking Light Firmware

Arduino-based firmware developed in the PlatformIO development environment for
the Arduino Nano Every.  


## Functional Specifications
### Electronics / PCB
* Powered by 9 V battery.
* Arduino Nano Every should be mounted to the board using a socket.
* Single-sided traces.
* Battery, switches potentiometers and LEDs should all be panel-mounted and connected to the board individually using 1x2 sockets.

### Firmware
The [firmware](src/main.cpp) in this repository is an example of how to approach the function above, but is **not** the solution for the specifications below!!  
*Component annotations below refer to the annotations on the electrical schematic that I want you to use, **not** the Arduino pin numbers.*
* SPST toggle power switch (ON/OFF).
* SPST push button switch (ON/OFF) to turn LEDs (`D1-2`) on for 5 seconds and LED
  (`D3`) for 10 seconds.
* Potentiometer to adjust brightness of LED `D1-2` using PWM (0-100%).
* Potentiometer to adjust blinking rate of LED `D3` (5-10 Hz).
* Have a "heartbeat" LED blinking at 1 Hz (`D4`) attached to pin `D13`.


### Enclosure
* Make an enclosure of a Blue Devil (or some other fun figure) where `D1-2` are the eyes and `D3` is the nose.
* The enclosure will be 3D printed.
* No linear dimension should exceed 10 cm.
* The enclosure should securely hold the PCB and battery internally.
* The power switch and push button should be securely panel mounted.
* The potentiometers should be securely panel mounted in the Blue Devil "ears".
* Any solder junctions between wires should be covered with heat shrink tubing.
* Use [threaded inserts](https://hackaday.com/2019/02/28/threading-3d-printed-parts-how-to-use-heat-set-inserts/) to attach parts together.
* You do *not* need to include mechanical drawings in your final submission.
* Include a directory in your git repository called `cad/` that includes:
  + A screenshot of your 3D model, saved as `model.png`.
  + The `model.stl` file for your 3D model that was 3D printed.


## Project Management / Implementation Expectations
* The entire project should be managed in a git repository called `one-shot-blinking-light`, with me (`mlp6`) and your TAs added as `Maintainers`.
  + Include your KiCad project in a subdirectory called `circuit/`.
* Questions about debugging code should be submitted as GitLab Issues that are assigned to your TAs (they can escalated to me).
* Final firmware should use "best" coding practices discussed in lecture, including:
  + Good firmware architecture:
    - `#include`
    - `#define` (MACROS for all pin mappings, specification values, etc.)
    - Function declarations
    - Global variable definitions
    - State machine framework
    - `setup()`
    - `loop()` using state machine framework
    - Function definitions
  + Interrupts for all "events"
 
## Circuit
* Please include your entire KiCad project in a directory called `circuit/` within your git repository.
* In addition to your KiCad project, please also include:
  + A screenshot of your schematic, saved as `schematic.png`.
  + A screenshot of your PCB layout, saved as `pcb.png`.
  + Passing `DRC.rpt` and `ERC.rpt` files.

The circuit below is **not** valid for the functions above, but just show a representation of some of the desired functionality.  

**Note, specifically, that this circuit lacks any of the required socket connections.**

<img src="circuit/schematic.png" alt="schematic" width="800"/>

### Arduino Nano Every Pinout
<img src="arduino_nano_every_pinout.png" alt="pinout" width="800"/>

## Testing
Create a `Testing.md` file in your repository that includes the following:
* Quantify the linear trend between your potentiometer setting and the brightness of `D1` and `D2`.
* Quantify the linear trend between your potentiometer setting and the blinking rate of `D3`.
* Quantify the accuracy of your heartbeat LED (`D4`) blinking at 1 Hz.
* Include all "raw" data measured during testing in your `Testing.md` file, along with a brief description of how the data were collected (so that someone couple replicate your measurements).
* Include all plots of analysis of your raw data in your `Testing.md` file.
* Discuss any disagreements between your testing results and the expected results from the specifications.

## What to Submit
* Make sure that your git repository containts the firmware, CAD and KiCad components described above.
* Create an annotated tag of your "final" commit for this project called `final`.
* Upload a zip archive of the `final`-tagged commit of your git repository to the final project assignment in Gradescope.

## Proposed Timeline (Latest Dates to Complete)
* March 22
  + Draft enclosure CAD -> 3D print
* March 29
  + Schematic & PCB layout
  + Rough fit in enclosure (CAD and mock in 3D print)
  + Finish first-pass firmware
* April 05 
  + Finalize PCB layout and submit for milling
  + Finalize enclosure CAD and 3D print
  + Test and finalize firmware (gather testing data)
* April 12
  + Solder PCB
  + Test PCB
  + Start device integration and assembly
* April 19
  + Last chance for redo PCBs and 3D prints
  + Finalize testing and documentation
* April 26
  + Final submission!
  + No late submissions accepted.

## Resources
* https://docs.platformio.org/en/stable/boards/atmelmegaavr/nano_every.html
* https://docs.arduino.cc/resources/datasheets/ABX00028-datasheet.pdf
* https://hackaday.com/2019/02/28/threading-3d-printed-parts-how-to-use-heat-set-inserts/

Copyright (c) [2024] [Mark L. Palmeri]
