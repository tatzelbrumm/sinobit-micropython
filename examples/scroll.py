# Simple message scrolling demo.  Will scroll the specified message across
# the display from right edge to left edge.  Try changing the message to
# to text with Unicode Latin & Latin-1 supplement characters!
# Author: Tony DiCola
import microbit
import sinobit

MESSAGE = '¿Hablas español? Parlez-vous Français?'

x = 11
width = sinobit.display.text_width(MESSAGE)
while True:
    sinobit.display.clear()
    sinobit.display.text(x, 0, MESSAGE)
    sinobit.display.write()
    microbit.sleep(100)
    x -= 1
    if x < -width:
        x = 11
