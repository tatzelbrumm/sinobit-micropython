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

-   **[Getting Started & Overview](https://learn.adafruit.com/sino-bit-micropython)** - Guide
    on the Adafruit Learning System to help get started using sino:bit MicroPython.  Start here!
-   **[Code Editor]({{ site.url }}{{ site.baseurl }}/editor/editor.html)** - Write sino:bit MicroPython code directly in your
    browser and download a firmware to run right on your sino:bit!  This is a
    port of the BBC micro:bit web editor to be compatible with the sino:bit
    MicroPythin firmware in this project.  If you want to program your sino:bit
    with MicroPython start here!
-   **[Releases](https://github.com/tdicola/sinobit-micropython/releases)** -
    Find the latest releases of sino:bit MicroPython on GitHub.
-   **[Source](https://github.com/tdicola/sinobit-micropython)** - Find the
    source for sino:bit MicroPython on GitHub.

## Example Programs

Here are a few examples that will load right in the code editor.  Ignore the
prompt for a password and just press enter, none is set. Click download
to retrieve a hex file you can drag to the sino:bit's drive and program it!

-   [Hello sino:bit message scroll](https://tdicola.github.io/sinobit-micropython/editor/editor.html?n=U2FsdGVkX19OZgL5SE%2FD4W00fsSqF5lku5amyGYUdMM%3D&c=U2FsdGVkX1%2FW6HbRDRo0VnM8WMiouQkKtrXZNg6PRPotusuRWXNPlT2g346t07JK&s=U2FsdGVkX1%2B%2BWgukDIVDp4%2BcmR8sooljcBRNItbSufr1ze%2BeNwwvl7554qmbu3oCDetd0oK6f0P%2FfGznJEIA2ZC1LkhKcVUeKdo1x68xUWd9UkoMGPiqrXdfkmO%2Fk7vc1VaDLGN5C2oWLKsTu16ITSVeN%2FF776Iv4hpUNs%2Fhb2DwIRN7zAiVzVyx13H%2BmhnXMLZSWi14OvXrwq8JKeqDk%2F%2FUx%2BoYoV%2FWg59ZRbqQUmM%3D&h=) - Simple scrolling message using the same code as a micro:bit.
-   [Arc Reactor](https://tdicola.github.io/sinobit-micropython/editor/editor.html?n=U2FsdGVkX1939aVlzc3G3ozmcjeEzDDlXMxv5Qb5A9g%3D&c=U2FsdGVkX189q%2FN%2BkOLdZjQDwjYUqo0PdYN%2B%2BQPGzr2pdkkFGjkiZppKLKVTOd6b&s=U2FsdGVkX1%2B81lkkyRNUSnF%2Bc%2BEgcPjX6OhP6UC2ULW%2B1MfLkyovadCsLtCQYKTp%2FuL3qizSemUeuHlaLQC%2Brjs8cFV1Fs4InCrENvUCE2K%2BXJVDBYC5ZdgLorOzYlInEkbovJjMrq28vEWlRFiX38Zo2NEksDcCTRg8FeZvXpMWucz7xR%2FsDZNaQdfFebK%2BvhugLLFpwjLY1nPFdbYmwS9caHx2tcsZ7%2BxIVTF6cACN2y2htaaV9iqRrzgQ722FiybQvK2kLPoTUF3D7S52fDt%2Bo7zLVrNPWH1L0d8kbiZm%2F%2BjOCc7HaMRJ5kTzMM87lQ6oXzR6yl9r8GvbJbOw9PFmoKBV2dovayieY%2FtA1Enh1JsxqWseAgwRdk640wY16QAGoWw1ShNdTjqZaB7U%2FTCxcYuvwFpT10ACkaEDDVl94sOFND%2BlmJAR3IYhkaYxNHoXaVZJLzzaEFJD%2FumRltsJXEvW90Dr1J9T8oqh4uNfLP3fo1BTi%2BX17KqJPW1vod%2FIhzhrJ8mNt1%2FcY77Fqx3PF%2FfkK4CuaS%2B5FSbTegLug8j0VY2cj1MBVSK39P6Qfn7X%2B5AoGkv956GEtYQS0kk0%2FjpX0nKQVJDTSfhNivS6%2Bzui8vwZVA2VoKk1WZQatil8SHFmMtgMAoygGqHdzwl2zwJ4R8ye9wzfgF1wElqGQJNcyi4etzif0fX9DY1EZI1I5ibbCFYCugNAYX3%2BdwUyPEQkdpJe3E0H%2Bqx%2BHe690ZlHeMCzthGA%2B22aynS4mJx98VyJDQdMr54Kdc8i6vAJbVFl4Wb6KV5mRjE42858CVqmUrluGIPdCaxUAiQAxqf3D9%2FhpmM2JwNhA6DHKUHV0clI08XbKvNCX4A%2F8MynlynL8lsv5ee0oMZAmn6DRx33yJd7Api%2F6zoLVrb0D%2FakCwCKdUrMdV4vosaam1gUwdx964X%2B9suVoN9TQ0zX8KMEJjhWR%2BuiCU8jIZRz2GlLpoFqeKSWcpoR8KdHilo0hxkcLuIm7q40REMF4fsZcdJOl6blXUqEafkBp0SrbWS1RLvDNZ%2Fqm82NMaw3e%2BGlAninHJasxX9mH2NuRLr9bCsegr9PMwIdAAbfCpGZKTA3Enmd5f1BaQ0Ve7sK5NAY2Va8dw4s%2BEpB6ZQ2cDFR1GG6RPc3zbeDDy3O59V1WiHz4PUvDPkl01cxs6bmJJGDmYdsrzNQeehyH7jDO%2F7nJq4MaiBjyImEwuy6Csb6Wm5Rp4tT%2F9%2Bp8m2SG22WPy0mHZCPWB4Y01sFN4nkY9ki%2BQlXccp%2B58aIbgFO%2Bz92UzBdKXv24GGFY0p7ywkWz9XPtnPOTvDqTCkRCRX%2F471aUA%2FbBoo9jZEqk86dAv%2BIoqTbM3%2BS5Kw%2BjPXihCHfNLBZojSgOyKpB2ZQsFS%2BNyT7ob11PRm8mw2LHXWYEsq6WB11xA67YmpR4FniEtW3PnHqFjuglOc2KS9zHAk8p5LKXLGFvV4urkrB7rAgCbZNxrIrkk5ybnAhZp%2FYPSCMbkPc5eyrabhP5GHJGv%2B8IAtLYdEn4ifr6%2FnE91PnIlYFBgA7Xsr5i8wcgh7BqTvTAIhIbqOFAitEdXBnu9Czit0338KuCL2trbis0rv5GiH4%2FKnSPGeda0ZMm4B99CQxDmNAQIRn%2BvcucXJPmffiS1CjbgSZ9iexYCd0%2Brvb1VqKQgz5scnUlus5IFeIUeAOhARColamsZy5AZcl7NJnaa50Mx1PnBH%2F0eZhQ3WU0RbuVwFh84EIQoqrNhCHHZN4MFVfMFSA9ete9LNec5JttV6ytP9GCuzhea1NzUGt0I8Rh8TSqm32BsIz0%2F5qHquk%2B2Qwerrdivcv5f70yVIPwBjyPhWF63O8v1p2rsv7MULorOZBeLhNB93f4lw5PdCd0kianQSpuug4pi3G%2B9l02sux1NZjMYMwlv6Vl3UcrAzp2zZbou5sXOq62tOWzK8iZ6Da4h3wc%2BDjk0%2FAi0fOxNplQY41XZQUD5SuEav8HktVjkhwZitcodQ5leQjaaHMWG6hs9sk%2F%2Fn%2F%2B7tKrEartE3oqPf6It6j3QLv%2Flarho%2F56QrHK0fhOwJo72oB3o97p%2F1F0Q3QRntbxDz9fywdHpkp0VnsSMcQeIKZyBR3s3Ox%2Fo%2B41KbHRWalgI5fk%2BYJqm37ruubOT71gUPnhFbU%2F1vdkTU%2Bg7rbFjLCJUN0I5iXA8cbJIa53IZyUVTaYvvwWm3csuwnovxQom%2B%2Bi0LUuYHNWd9D9qSlvDDfxO1ToP%2BQgZZzH8igCBYN%2BYe9SSJIhLWwpNHsmsu%2F%2FSJKWe2%2B66j%2B0PZPSqGGK6OkTX1Po%2FpJfr8JXo89Llb8AvaL5hHJ0rOWU97t2xzPasSUasgessczgNFyolAOin36NFuyVUWqus2PCOysLezo0wIz5sUYIxkB23sLUdRiljjmNc99zOTZBJFWICi19vBKlYymyT2gHXNi9QIqMkcDzM%2B2%2BPNHa1xyr7XF6yZNb498p5ydx%2Fefdq5%2Bop2gSL8FpL2yxR6NWgS6bGGnu%2BJwWIW6OBOHmgCn%2BSUJn0NPh8B11EKpZ4WZ1%2FWZoRctSsRtZSgKfk4BFUxAqMp1Hx959CXkSrFsb%2BNhORlLeWN%2BvMsT2gV2pzjoRgsM4o8Inj0lW2h91PK0PTvdMp0NLBT%2FGr1hlEcXBSYl1EKcllYizb0ibpYo1QFDzJDHaL0MHd8HLf14lZNZZofibstSDw8jjX%2BUqLDWSgOT36EZcthh5hhYgX%2F0%2FVIlaJFQP0neovVso9stqcSH%2FKYRrlAqZAZjXZhnunEtdAhlH%2BXgOfLapKR26lkdhNFAwxybrTQeoJcDXkT9xxDkQK%2BKx9b8nBsdQHNQp0k0o8TVDpWULY0Arani3nf5EQp9RLljO%2FlDkuYlUJy7NToC%2FOH7dMIgUNx3AZwvuB0VU8szbpRGUmIidw490XTpV4ApzKBdC%2BZHaqDjSWkMRVPJGeuy7XH%2FjnLCc4UMDF143FI1R9agTivT2cFePxePoD9zCZkvaUeFllPiYsJRfyz%2BPCHQs%2F0LCY%2Fu3BwzrSBj9lmAf9X3znolHUlFZbNxSwKTg2zS3rApDn2N2KxtSC9zvzNP%2B7OCY4zZi%2B%2BbJeQ8MWIpPEHsdX7acORJLewA18%2FgcHKX5AKkswESA7Pe28T%2BwF643F7vtTxwtipYA3mk%2FaWO0uf5uqFwnUkdn7t%2Bk7KBKMqdcSnZ0AUzQtgTsjgNX7mtiixlKPl%2BYxs4g57J1i1jEfSbW6gKqUtcYTkD9BBcgHEvq1SPWN%2FATL4Qcc87SWdQFrCkh58Lufd9b6VVG%2BTJU5kSEJcxlPvw3l3LkA3kb1Bq6%2BLEqHky79RtWmE55heBkqsXTBKodAJdjhmv5Kxj1e9mR0BkprU7fLXRGg5C8TBURo40hx%2Bsrq6rv30YnkxrYB5q%2FOEoezSQYD9%2BwzP0ad0lcOQwMyQ7e7rLCBJwRc%2FiB%2Be89ZkEUOITlGaohVhrqwjAXYi50KjAYivqOw60%2FGzu2poFcxfLWVaH1OBWbKQCYAqNp2IHWVJxnLzKXs3cxBiFl5VtWlkEu830wbgpYUwkDY%2FfbFdprKEOTprcyETDbrctzZ7XtgjmFymtzuV33gbTgaifkwmon6sEsoaElmRCDBb4oLmTN6JnHupsz3VQ%2FRsHi8C3A3jdkGxIXggifnotu4GR%2FBcfGtOA1N2ONfOdYlnWOwl4sZm6a60PEEMBSLFp1k3DA%2Bh5zlxKSjldKvXT2VSjQq0yexqt38C8l8smyv%2BYq%2FxvT%2BgiPufgec%2BS2R82f%2FiZdq%2F7btyeT7z4ZYKS5DX9JJ%2FHy8l18zgdGpP0DMOFCs35Qywq694pq86osOywt2Do3MQmtijKpcgBorRGaixgtzJxflSn14oJunaIi1%2B8GfjI52I4yj%2FEUK39s40PYrL1X9L3rlkApmzRQHKm9JSqhbSRDi2NT9V6COqTEa%2BdiFlIq%2BElJ3yxkr6tLoWT9%2FEjvekmVqoQHWLH3leQcNQ3FRi%2Fwv7WP4EOTHrDeCkpBV8zmVEA8vu95QYtC9Osf2c3Wo9Mn8x%2BY5ZMXXmN6NDSRn35%2FLIdfpH7N2KjQVbDmjfbXwcKCOWQN6LIxnW%2FyCZ7rMCMRhH0poELhQsAfRiSAk9WXgN%2B5TGSIvFVhGBxGLQqkMsBXhXNYoWQiz%2ByJoaiwM%2BBIE5Cnf6hr7nlCDxYAxxsgxc0wbnF7%2BufG%2BKb9Ob08DkvVf%2BTxsVy6Zo5mysmRJxzkMl8EvBupsDBIqmTEYALBlPrwK%2FTonTqJhrF30U0II8mD3xJFtzXVUJVSuvJaDxgeYU40yJkLyhYpGa6hY%2B2GMMVyhdrE73kzrEbadKYQhmoGXlVqGcuo1Q2cogiDpGlGagJJ3M3KMfJEv8w0i9hPc0JTldcafjOPGBsgjQPRxWqMHQ3igd%2Fllj6d9C7LK0woJtw9fkacUzfu8aqjNi21%2Bz0KaSRJmSrfSU%2B4twrWpItDoIRTel9Q71GwZYZcuLhN13pZbaqkecG7NW7QSNSjgYqxzESo0mQOwbpylKIUAARbr60iWaTY3qOUoCwqhNVGWrijTV1h54oIcjhmLlAeSZf9u3sk99V%2Fm3SDlJJ0hhXzQLvd%2FtMmt41s6gfCCpiZApKAHrkz17%2BxQjTw7B0B%2ByrqYWAhGu2JNZ7h3MJBycOWuP9UYqlIXHmcbvl0UyUt%2BnuK24A83ZrXipCbVuhUkurhYmPpNeJG77zLOwn4zuMdZkOoMu2lhSuVE%2BhAQtvoe696LoV3uXzFcC9j%2B%2B53vPieYfrNWHY5UEVGta7rB1lqeWMNx7VtZsBEdGsLALJHsVllcppZQB3FbtYaqUgbU9WINiHjDFlUj2V96n5YrNhx%2BIHtNf3WM57hMhlMzWE8R0r%2BgCnPSjdpef72xsx%2Ba%2BFP8PWyuUJfL%2Fzf1UYOepajZmW4ZHKqd6FQmelIYVY7BJ4cX3RwyRrQ3Cfg1gfn5fFk5NhQfKopuvVzaRDIXphx8kr8q%2FUWTTb3Yh%2BQ87eLP0xyU4r8A9Yb3ZijKLTk7UCXyUzZcTeBpuJ%2FzchO64NFOAxYeRTkx50xDFPyBrBNGyC2T52KWBvMKMxTcyKUo05vdaPCO0NzrR6ho5n2Osr1DKAr0ScWs7zEuUrJ27xBBXubkqY7MZyxLQI%2BGaITtpO%2FTR%2FhN5hD9JY5n%2F%2BzfKdXrs%2B2uJgyA%2BxTU9DcFEp8q61I8NEpVhxQtr7YFGMctUqvI8BrvezG6g1FNeV0qMJj5W0GiJprqjk%2BtiAo0C9A3Ie6hWpESCV74NoWLGD822gWeC%2FWSJKyon81CnwtsUkfMIEJSh03UTMRirHG9Mr2cHMaA%3D%3D&h=) - Fun 'arc reactor' animation of boxes that animate moving towards the center of the display.  This uses the full 12x12 LED matrix!
-   [All the examples in the GitHub repository.](https://github.com/tdicola/sinobit-micropython/tree/master/examples)

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
    -   Consider using this 12x12 font as a base: https://github.com/SolidZORO/zpix-pixel-font
    -   Good suggestion to scan source script (like in web editor) and extract
        the set of unicode characters in use to inject into the firmware:
        https://twitter.com/sceptic_int/status/963668006625861635  This
        could allow a wide range of characters but without dynamic use of them.
    -   Right now have about ~10kb free in firmware (with no loss of modules
        or features yet).  If can get up to ~100kb free could fit ~6k characters.
    -   Good reference material too: https://r12a.github.io/scripts/chinese/
        And the GB2312 standard: https://en.wikipedia.org/wiki/GB_2312  Also DB
        of characters here: http://hanzidb.org/character-list/general-standard
    -   Also consider other languages as the zpix font includes Japanese and
        accented English characters.  There's a Hindi font here too: https://github.com/nishapoyarekar/Sinobit/tree/indic
    -   Entire unicode font in 16x16 and 8x16 size: http://unifoundry.com/unifont.html
    -   __Language support is starting to come together!  See a preview here: 
        https://twitter.com/tdicola/status/965489097769828352__
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
    -   microbit-dal usage:
        -   Font data (MicroBitFont.cpp)
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
    -   _This is done!  See the code editor link at the top of the page.  Some work remains to be done to clean the wording up and make it sino:bit specific._

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
