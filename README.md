# PixelRingAnimator
A library to animate neo pixel rings from Adafruit with predefined animation
patterns.

The neo pixel rings available from Adafruit are essentially neo pixel strips
arranged in a circular configuration. But being circular means that one can
create animations that are fun and can be used to communicate state. I use
them on my robots to indicate when the robot is doing something like moving
or thinking. I have collected different animation patterns into the
PixelRingAnimator class for use more generally and not specific to any one
project.

## Dependencies
This library is dependent on the existing Adafruit NeoPixel library.
A version of the library probably came with your Arduino IDE instance or can
be downloaded using the IDE (at least version 2.0 support this). If not, the
latest version of the library can be
[downloaded from github here](https://github.com/adafruit/Adafruit_NeoPixel).

## Example
An example, named 'simple,' accompanies this library, and demonstrates the
basic usage.

Be sure to call the <code>start()</code> method before calling other methods
to animate patterns. The method takes the pin that the pixel ring data lead
is connected to, the number of pixels on the ring, and the type of neopixel
(more on this in a moment). It also takes an optional parameter to define the
'base' pixel. The base pixel will be treated as pixel zero for patterns where
it matters, essentially the pixel where the pattern will start.

The type of neopixel being used on your ring can be tricky to figure out, at
least it was for me. There is generally no indication on the ring as to what
byte order should be used for the neo pixels. You may have to experiment until
you find the right order. See the top of the
[Adafruit_NeoPixel.h file](https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.h)
for all the valid permutations and constants (there are a lot). Then try to
set just one color, like red, Go through the permutations until you see that
color. Then green, then blue. Tedious, but once you have it you are golden.
Unless you have an old ring, you can probably assume that it is 800Khz
(ie NEO_KHZ800). 

The rate at which the pattern is animated on the ring is determined by the
rate at which the <code>update()</code> method is called. Calling
<code>update()</code> will advance the pattern a single iteration in the
pattern. You generally call the <code>update()</code> method from the
<code>loop()</code> method in the Arduino sketch. The example calls it
every 50 milliseconds.

## Other helpful stuff
I also have some other libraries you might find useful for Arduino programming.

- [ArduinoLogging](https://github.com/markwomack/ArduinoLogging) -
  A library for controlling debug message output for debugging.
- [TaskManager](https://github.com/markwomack/TaskManager) - A
  library to manage executing tasks at specific rates (useful
  to call the PixelRingAnimator update() method at a specific rate).
