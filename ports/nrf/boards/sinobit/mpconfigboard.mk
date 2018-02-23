MCU_SERIES = m0
MCU_VARIANT = nrf51
MCU_SUB_VARIANT = nrf51822
LD_FILE = boards/nrf51x22_256k_16k.ld
FLASHER = pyocd

# Disable lto type mismatch warning because of this bug in GCC:
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81440
CFLAGS += -Wno-lto-type-mismatch
