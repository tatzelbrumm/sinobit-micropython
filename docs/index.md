---
layout: default
---
The [sino:bit](https://github.com/sinobitorg/hardware) is a single board
computer for education in China designed by [Naomi
Wu](https://twitter.com/realsexycyborg?lang=en) and
[Elecrow](https://www.elecrow.com). The design of the sino:bit is similar  to
the Calliopie Mini and BBC micro:bit but customized with features like an
extended 12x12 LED matrix that can display Asian characters and more.
[MicroPython](http://www.micropython.org) is a version of the Python programming
language created by Damien George that can run on small single board computers
like the BBC micro:bit. This project is a fork of MicroPython for the BBC
micro:bit that's intended to run and support the hardware of the sino:bit.

-   **[Code Editor]({{ site.url }}{{ site.baseurl }}/editor/editor.html)** - Write sino:bit MicroPython code directly in your
    browser and download a firmware to run right on your sino:bit!  This is a
    port of the BBC micro:bit web editor to be compatible with the sino:bit
    MicroPythin firmware in this project.  If you want to program your sino:bit
    with MicroPython start here!
-   **[Releases](https://github.com/tdicola/sinobit-micropython/releases)** -
    Find the latest releases of sino:bit MicroPython on GitHub.
-   **[Source](https://github.com/tdicola/sinobit-micropython)** - Find the
    source for sino:bit MicroPython on GitHub.

## More sino:bit Information

-   [sino:bit GitHub home (schematics and more)](https://github.com/sinobitorg/hardware)
-   [sino:bit Product Page (where to buy one)](https://www.elecrow.com/sino-bit-v1-0.html)
-   [sino:bit with Arduino Guide](https://learn.adafruit.com/sino-bit-with-arduino/dastels-overview)
-   [MCU Monday ft. Naomi Wu and the sino:bit](https://www.youtube.com/watch?v=81JzsTOmM28)
-   [sino:bit Open Source coding projects for children by Keith Parkins](https://medium.com/dark-mountain/sino-bit-3691a7decf68)

## BBC micro:bit and MicroPython Information

-   [BBC micro:bit MicroPython documentation](https://microbit-micropython.readthedocs.io/en/latest/)
-   [BBC micro:bit Datasheets](http://tech.microbit.org/hardware/schematic/)
-   [BBC micro:bit Schematics](https://github.com/bbcmicrobit/hardware)
-   [MicroPython project](http://www.micropython.org/)
-   [BBC micro:bit MicroPython guide](http://microbit.org/guide/python/)
-   [Adafruit's MicroPython guides](http://learn.adafruit.com/category/micropython)
-   [Adafruit's What is MicroPython? guide](https://learn.adafruit.com/micropython-basics-what-is-micropython)

Note this project is not related to the [BBC micro:bit MicroPython
port](https://github.com/bbcmicrobit/micropython) (beyond being the starting
point for the code of this port). Questions, issues, or support for this port
should not be directed at the micro:bit community.

## Project State

This is currently an experimental / brand new port of MicroPython.  Be
aware it is under development and may not work as expected!  No guarantees
are made with regard to API stability or compatibility, support, or
frequency of updates and releases.

When they are available pre-built binary firmwares will be published in the
releases tab of this GitHub repository.  These are the .hex files you want
to drag onto the sino:bit's MICROBIT drive to load the firmware.

## Project Goals

_This is currently a brain dump of some ideas, goals, to-dos for this project._

-   Add support for the 12x12 LED matrix to the MicroPython firmware.  Support
    basic pixel drawing operations (clear, fill, set pixel, draw line, draw
    circle, etc.).
    -   _This is now implemented with the 0.0.1 release! Support for line,
        circle and other primitive drawing is still to come._
-   Add support for Chinese text rendering on the matrix.  There is a lot of
    work to investigate here, particularly with text encodings, character sets,
    character representations and rendering.
-   Backwards compatibility with the micro:bit 5x5 LED display.  In theory it's
    possible to upsample drawing commands intended for the micro:bit's 5x5
    display to instead target 2x2 pixel chunks on the 12x12 display (with an
    unused single pixel frame around the edge).  This would make it possible for
    existing micro:bit MicroPython code to 'just work' on the sino:bit and display
    with large pixels.
    -   _This is now implemented with the 0.0.2 release!  The microbit.display
        module is implemented with upsampling to the larger sino:bit matrix.
        Note that individual pixel brightness values are ignored/lost as the
        sino:bit display doesn't support it._
-   Investigate untangling and removing hardware access layer dependencies.  It
    appears there are 3 hardware access layers being used: Nordic's nRF layer,
    mBed's classic layer, and a Lancaster University micro:bit device access layer.
    As the sino:bit no longer needs complex 5x5 LED matrix driving it is unclear
    the need for some of these layers like the Lancaster University DAL.  Perhaps
    dropping down to a basic usage of Nordic or mBed's hardware access layer would
    save flash space.  Support for the board peripherals like accelerometer and
    magnetomer could be added as simple C extensions.
-   Work on renaming from micro:bit to sino:bit internally.  The microbit module
    should ideally be retained for compatibility with existing micro:bit MicroPython
    code.  However much of the project structure, code files, etc. currently still
    use the micro:bit nomenclature.
-   Port the BBC micro:bit web editor to support the sino:bit.  This editor works by
    appending a MicroPython script to a certain location in the MicroPython firmware
    hex file (thus it's easy for an entirely web-based/JavaScript app to 'compile'
    code for the board).  It should just be a matter of taking the web editor code
    and changing the hex file it uses to point at the sino:bit version here, along
    with minor tweaks to the UI language, etc.  Ideally this could just be hosted
    straight from this GitHub repository using GitHub's jekyll-based pages rendering.

## Differences with the BBC micro:bit

-   The major difference between the BBC micro:bit and sino:bit is the
    LED matrix display.  The sino:bit has a large 12x12 (144 total) LED
    matrix that is driven by a dedicated Holtek HT1632C LED driver.  In
    comparison the BBC micro:bit has a 5x5 LED that is driven directly
    by the GPIO pins of the processor.  As a result the sino:bit has the
    potential to display much more information than the micro:bit, and
    the dedicated LED driver relieves some of the processing burden of
    driving the display from the CPU.  However note the HT1632C does not
    support driving the brightness level of individual LEDs and instead can
    only brighten or dim the entire display.

-   The sino:bit includes grove-style connectors to break out I2C and
    serial UART connections with other sensors.  In addition an unpopulated
    26-pin header provides access to all GPIOs vs. a card edge connector
    on one side of the micro:bit.  The sino:bit also includes 6 large
    GPIO access holes (P0-P5) vs. only 3 (P0-P2) on the micro:bit.