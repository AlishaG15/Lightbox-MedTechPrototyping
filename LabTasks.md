# Incremental Lab Tasks

## Setup GitLab Repository
* Setup a new GitLab repository called `one-shot-blinking-light`
* Add Dr. Palmeri (`mlp6`) as a `Maintainer`.
* Create an Issue assigned to Dr. Palmeri asking him record your repository URL.

## Astable (Blinking) Light Output
* Implement an astable blinking LED at a specified duty cycle in your firmware.
* `D13` is the pin associated with the on-board LED on the Arduino Nano.
* The LED blink rate and duty cycle should be specified as macros.
* Do not use the `delay()` function; conditionally test the current time against the next blink time.
* Make measurements on the oscilloscope verifiying your blink rate and duty cycle.

## Button-Triggered One-Shot Blinking Light Output
* Configure a digital pin as `INPUT_PULLUP`.
* Attached an interrupt to that digital input pin.
* Write a callback function in response to the ISR triggered by pressing the button that toggles the state of a Boolean variable to represent that a button press occurred.
* In your main loop, test the state of the Boolean variable to determine if the LED should start blinking at the frequency and duty cycle you verified last week.
* Have the button-initiated blinking last for a macro-specified one-shot duration.
* Verify your one-shot duration, blink frequency and duty cycle on the oscilloscope.  *Remember that you can emulate your push button behavior by transiently attaching your digital input pin to ground.*
* Commit your code to your repository and push it to GitLab.
* Take a screenshot of your verified one-shot duration, blink frequency and duty cycle on the oscilloscope and add it to your repository.  You can take a picture of the screen with your cell phone and add that image and save a screenshot with a USB memory stick.
* Create an annotated tag called `v0.1` that contains your verified firmware and your oscilloscope screenshot and push it to GitLab.
* Create a new Issue for Dr. Palmeri to review your code and oscilloscope screenshot.
