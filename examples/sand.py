# Digital sand demo uses the accelerometer to move sand particiles in a
# realistic way.  Tilt the board to see the sand grains tumble around and light
# up LEDs.  Based on the code created by Phil Burgess and Dave Astels, see:
#   https://learn.adafruit.com/digital-sand-dotstar-circuitpython-edition/code
#   https://learn.adafruit.com/animated-led-sand
# Ported to sino:bit by Tony DiCola
#
# The MIT License (MIT)
#
# Copyright (c) 2018 Tony DiCola
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
import math
import random

import microbit
import sinobit


# Configuration:
GRAINS   = 20    # Number of grains of sand
WIDTH    = 12    # Display width in pixels
HEIGHT   = 12    # Display height in pixels


# Class to represent the position of each grain.
class Grain:

    def __init__(self):
        self.x = 0
        self.y = 0
        self.vx = 0
        self.vy = 0

# Helper to find a grain at x, y within the occupied_bits list.
def index_of_xy(x, y):
    return (y >> 8)  * WIDTH + (x >> 8)

# Global state
max_x = WIDTH * 256 - 1    # Grain coordinates are 256 times the pixel
max_y = HEIGHT * 256 - 1   # coordinates to allow finer sub-pixel movements.
grains = [Grain() for _ in range(GRAINS)]
occupied_bits = [False for _ in range(WIDTH * HEIGHT)]
oldidx = 0
newidx = 0
delta = 0
newx = 0
newy = 0

# Randomly place grains to start.  Go through each grain and pick random
# positions until one is found.  Start with no initial velocity too.
for g in grains:
    placed = False
    while not placed:
        g.x = random.randint(0, max_x)
        g.y = random.randint(0, max_y)
        placed = not occupied_bits[index_of_xy(g.x, g.y)]
    occupied_bits[index_of_xy(g.x, g.y)] = True

# Main loop.
while True:
    # Draw each grain.
    sinobit.display.clear()
    for g in grains:
        x = g.x >> 8  # Convert from grain coordinates to pixel coordinates by
        y = g.y >> 8  # dividing by 256.
        sinobit.display.set_pixel(x, y, True)
    sinobit.display.write()

    # Read accelerometer...
    f_x, f_y, f_z = microbit.accelerometer.get_values()
    # sinobit accelerometer returns values in signed -1024 to 1024 values
    # that are millig's.  We'll divide by 8 to get a value in the -127 to 127
    # range for the sand coordinates.  We invert the y axis to match the
    # current display orientation too.
    f_y *= -1                         # Invert y
    ax = f_x >> 3                     # Transform accelerometer axes
    ay = f_y >> 3                     # to grain coordinate space (divide by 8)
    az = abs(f_z) >> 6                # Random motion factor grabs a few top
                                      # bits from Z axis.
    az = 1 if (az >= 3) else (4 - az) # Clip & invert
    ax -= az                          # Subtract motion factor from X, Y
    ay -= az
    az2 = (az << 1) + 1         # Range of random motion to add back in

    # ...and apply 2D accel vector to grain velocities...
    v2 = 0                      # Velocity squared
    v = 0.0                     # Absolute velociy
    for g in grains:
        g.vx += ax + random.randint(0, az2) # A little randomness makes
        g.vy += ay + random.randint(0, az2) # tall stacks topple better!

        # Terminal velocity (in any direction) is 256 units -- equal to
        # 1 pixel -- which keeps moving grains from passing through each other
        # and other such mayhem.  Though it takes some extra math, velocity is
        # clipped as a 2D vector (not separately-limited X & Y) so that
        # diagonal movement isn't faster

        v2 = g.vx * g.vx + g.vy * g.vy
        if v2 > 65536:                    # If v^2 > 65536, then v > 256
            v = math.floor(math.sqrt(v2)) # Velocity vector magnitude
            g.vx = (g.vx // v) << 8       # Maintain heading
            g.vy = (g.vy // v) << 8       # Limit magnitude

    # ...then update position of each grain, one at a time, checking for
    # collisions and having them react.  This really seems like it shouldn't
    # work, as only one grain is considered at a time while the rest are
    # regarded as stationary.  Yet this naive algorithm, taking many not-
    # technically-quite-correct steps, and repeated quickly enough,
    # visually integrates into something that somewhat resembles physics.
    # (I'd initially tried implementing this as a bunch of concurrent and
    # "realistic" elastic collisions among circular grains, but the
    # calculations and volument of code quickly got out of hand for both
    # the tiny 8-bit AVR microcontroller and my tiny dinosaur brain.)

    for g in grains:
        newx = g.x + g.vx       # New position in grain space
        newy = g.y + g.vy
        if newx > max_x:        # If grain would go out of bounds
            newx = max_x        # keep it inside, and
            g.vx //= -2         # give a slight bounce off the wall
        elif newx < 0:
            newx = 0
            g.vx //= -2
        if newy > max_y:
            newy = max_y
            g.vy //= -2
        elif newy < 0:
            newy = 0
            g.vy //= -2

        oldidx = index_of_xy(g.x, g.y)            # prior pixel
        newidx = index_of_xy(newx, newy)          # new pixel
        if oldidx != newidx and occupied_bits[newidx]: # If grain is moving to a new pixel...
                                                       # but if that pixel is already occupied...
            delta = abs(newidx - oldidx)          # What direction when blocked?
            if delta == 1:                        # 1 pixel left or right
                newx = g.x                        # cancel x motion
                g.vx //= -2                       # and bounce X velocity (Y is ok)
                newidx = oldidx                   # no pixel change
            elif delta == WIDTH:                  # 1 pixel up or down
                newy = g.y                        # cancel Y motion
                g.vy //= -2                       # and bounce Y velocity (X is ok)
                newidx = oldidx                   # no pixel change
            else:                                 # Diagonal intersection is more tricky...
                # Try skidding along just one axis of motion if possible (start w/
                # faster axis).  Because we've already established that diagonal
                # (both-axis) motion is occurring, moving on either axis alone WILL
                # change the pixel index, no need to check that again.
                if abs(g.vx) > abs(g.vy): # x axis is faster
                    newidx = index_of_xy(newx, g.y)
                    if not occupied_bits[newidx]: # that pixel is free, take it! But...
                        newy = g.y           # cancel Y motion
                        g.vy //= -2          # and bounce Y velocity
                    else:                    # X pixel is taken, so try Y...
                        newidx = index_of_xy(g.x, newy)
                        if not occupied_bits[newidx]: # Pixel is free, take it, but first...
                            newx = g.x           # Cancel X motion
                            g.vx //= -2          # Bounce X velocity
                        else:                    # both spots are occupied
                            newx = g.x           # Cancel X & Y motion
                            newy = g.y
                            g.vx //= -2 # Bounce X & Y velocity
                            g.vy //= -2
                            newidx = oldidx # Not moving
                else:                       # y axis is faster. start there
                    newidx = index_of_xy(g.x, newy)
                    if not occupied_bits[newidx]: # Pixel's free! Take it! But...
                        newx = g.x           # Cancel X motion
                        g.vx //= -2          # Bounce X velocity
                    else:                    # Y pixel is taken, so try X...
                        newidx = index_of_xy(newx, g.y)
                        if not occupied_bits[newidx]: # Pixel is free, take it, but first...
                            newy = g.y           # cancel Y motion
                            g.vy //= -2          # and bounce Y velocity
                        else:                    # both spots are occupied
                            newx = g.x           # Cancel X & Y motion
                            newy = g.y
                            g.vx //= -2 # Bounce X & Y velocity
                            g.vy //= -2
                            newidx = oldidx # Not moving
        occupied_bits[oldidx] = False
        occupied_bits[newidx] = True
        g.x = newx
        g.y = newy
