#!/bin/sh
# Script that runs during Yotta's preGenerate step (right before CMake config
# is built).  This is a good place to put code generation and other pre-build
# commands.

# Invoke cog to generate code in the mpconfigport.h header.
cog.py -r -D config="$YOTTA_MERGED_CONFIG_FILE" ./inc/microbit/mpconfigport.h
