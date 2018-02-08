# sinobit 'Arc Reactor' Animation
# This will animate a 2 pixel wide rectangle that shrinks down across the
# entire 12x12 display.  The brightness level fades away over time at each step
# for a nicer effect too.
# Author: Tony DiCola

# Import the microbit module to access its sleep delay function.
# You can use all the other parts of the microbit module too except for its
# display module (for now!).  Instead see the sinobit module below for access
# to the display.
import microbit

# Import the display submodule from the sinobit module to write to the display.
from sinobit import display


# Define some functions in Python code to draw straight lines and boxes.
def hline(x, y, width, color):
    # Horizontal line at x, y position and width pixels to the right.
    # Color is a boolean true/false for on or off pixels.
    for i in range(x, x+width):
        # The set_pixel function will set a pixel at the provided x, y position
        # to on or off with the provided true/false color value.
        # Remember you won't see the pixel LED turn on/off until write is
        # called!
        display.set_pixel(i, y, color)

def vline(x, y, height, color):
    # Vertical line at x, y position and height pixels down.
    # Color is a boolean true/false for on or off pixels.
    for i in range(y, y+height):
        display.set_pixel(x, i, color)

def box(x, y, width, height, color):
    # Draw a 1 pixel wide box with upper left corner at x, y and of the
    # specified width and height of pixels.  Color is a boolean true/false
    # for turning the pixels on or off.
    hline(x, y, width, color)
    hline(x, y+height-1, width, color)
    vline(x, y, height, color)
    vline(x+width-1, y, height, color)

# Clear the display.  Note that all the display commands just update the
# internal memory and have to be followed by a write call to update the LEDs
# with the new memory value.  This way you can make a lot of pixel changes at
# once and then write them all in one call (as opposed to seeing all the pixel
# updates as they occur).
display.clear()
display.write()

# Main loop will run the code inside it forever:
while True:
    # We'll count from 0 to 5 to move the rectangle from a starting x, y
    # of 0, 0 down to 5, 5.
    for i in range(6):
        # First clear the display.  This is another way to do it by calling
        # fill and passing a color boolean to turn off the pixels (but you
        # could instead pass true to turn them all on!).
        display.fill(False)
        # Calculate the size of the square so that it shrinks down with
        # each step.
        size = 12-2*i
        # Draw the square.
        box(i, i, size, size, True)
        # If we're past the first iteration draw a second square behind this
        # one to double the size of the box.
        if i > 0:
            box(i-1, i-1, size+2, size+2, True)
        # Finally make sure to call write on the display to push out all the
        # pixels that were set with the box drawing commands above.  This will
        # turn on and off the appropriate LEDs to draw this frame of the
        # animation.
        display.write()
        # Once the pixels are lit you can change their brightness (you _don't_
        # have to call write after changing brightness, it will instantly
        # update!).  There are 16 brightness levels from 0 (lowest) to 15
        # (maximum, the defaul).  We'll loop down from 15 to 0 to dim the
        # square that was just drawn and make it appear to fade away over time.
        for b in range(15, -1, -1):
            display.brightness(b)
            # Delay at each brightness for a longer and longer period of time.
            # This means the lower brightness values will linger and give a
            # nicer exponential decay to brightness.  The sleep function will
            # pause for a number of milliseconds, like the delay function in
            # Arduino.
            microbit.sleep(17-b)
