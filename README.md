MicroPython for the sino:bit
============================

The sino:bit is a single board computer for education in China designed
by Naomi Wu and Elecrow: https://github.com/sinobitorg/hardware  The
design of the sino:bit is similar to the Calliopie Mini and BBC micro:bit
but customized with features like an extended 12x12 LED matrix that can
display Asian characters and more.  MicroPython is a version of the
Python programming language created by Damien George that can run on small
single board computers like the BBC micro:bit.  This project is a fork of
MicroPython for the BBC micro:bit that's intended to run and support the
hardware of the sino:bit.  You can control the sino:bit hardware from
Python code!

**[See the project homepage for more details on the project, including a web-based code editor to write and run code on your sino:bit!](http://tdicola.github.io/sinobit-micropython/)**

Building the Firmware
---------------------

_Note this information is inherited from the BBC micro:bit MicroPython repository
and mostly works.  Be warned you really need to have a dedicated Linux or POSIX-based
build environment right now to build the firmware.  Vagrant-based VMs currently seem
to have a lot of trouble (Ubuntu 12.04 LTS has old packages, 16.04 LTS has bugs with
Vagrant integration).  Windows and the Windows 10 Linux subsystem is a total unknown.
You have been warned._

**Be sure to clone this repository with the --recursive option to make sure necessary
submodules are downloaded.  Alternatively you can run git submodule init after cloning:***
```
git clone --recursive https://github.com/tdicola/sinobit-micropython.git
```
or:
```
git clone https://github.com/tdicola/sinobit-micropython.git
cd sinobit-micropython
git submodule init
git submodule update
```

The source code is a yotta application and needs yotta to build, along
with an ARM compiler toolchain (eg arm-none-eabi-gcc and friends).

Ubuntu users can install the needed packages using:
```
sudo add-apt-repository -y ppa:team-gcc-arm-embedded
sudo add-apt-repository -y ppa:pmiller-opensource/ppa
sudo apt-get update
sudo apt-get install cmake ninja-build gcc-arm-none-eabi srecord libssl-dev
pip3 install yotta cogapp
```

Mac OSX users can install some of the needed pacakges with [Homebrew](https://brew.sh/).
In addition you will need the [GCC ARM embedded toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
downloaded and its /bin/ directory in your path. Other depdencies can be installed with
Homebrew and Python's pip package manager (_this is rough information that needs to be verified_):
```
brew install cmake ninja srecord python3
pip3 install yotta cogapp
```

Once all packages are installed, use yotta to build.  You will need an ARM
mbed account to complete the first command, and will be prompted to create one
as a part of the process.

- Use target bbc-microbit-classic-gcc-nosd:

  ```
  yt target bbc-microbit-classic-gcc-nosd
  ```

- Run yotta update to fetch remote assets:

  ```
  yt up
  ```

- Start the build:

  ```
  yt build
  ```

The resulting microbit-micropython.hex file to flash onto the device can be
found in the build/bbc-microbit-classic-gcc-nosd/source from the root of the
repository.

There is a Makefile provided that does some extra preprocessing of the source,
which is needed only if you add new interned strings to qstrdefsport.h.  The
Makefile also puts the resulting firmware at build/firmware.hex, and includes
some convenience targets.

How to use
==========

Upon reset you will have a REPL on the USB CDC serial port, with baudrate
115200 (eg picocom /dev/ttyACM0 -b 115200).

Then try:

    >>> import microbit
    >>> microbit.display.scroll('hello!')
    >>> microbit.button_a.is_pressed()
    >>> dir(microbit)

Tab completion works and is very useful!

You can also [use a tool like ampy to copy scripts to the board and run them](https://learn.adafruit.com/micropython-basics-load-files-and-run-code/overview).

Be brave! Break things! Learn and have fun!
