# Compile flags to add that optimize the size of generated binaries.
# This is added to the CMake config that yotta generates, see:
# http://docs.yottabuild.org/reference/buildsystem.html
# (ignore the example in the docs, you MUST use the append property syntax
# below or else the compile flags and such set by dependencies are obliterated!)
#
# Currently these optimizations are enabled:
# - Os - optimize for binary size
# - DNDEBUG - remove asserts
#
# Note that -flto link time optimizations do NOT work in the resulting binary.
# Need to investigate later what's up but it won't boot to REPL.  Shaves a few
# kilobytes off the firmware size!
set_property(TARGET sinobit-micropython
             APPEND PROPERTY
             COMPILE_FLAGS "-Os -DNDEBUG")
set_property(TARGET sinobit-micropython
             APPEND PROPERTY
             LINK_FLAGS "-Os -DNDEBUG")
