# LD_FILE = boards/samd51x19-bootloader.ld
USB_VID = 0x04D8
USB_PID = 0xED94
USB_PRODUCT = "pycubed-mini"
USB_MANUFACTURER = "maholli"

CHIP_VARIANT = SAMD51G19A
CHIP_FAMILY = samd51

INTERNAL_FLASH_FILESYSTEM = 1
LONGINT_IMPL = MPZ

# Not needed.
CIRCUITPY_AUDIOBUSIO = 0
CIRCUITPY_AUDIOMIXER = 0
CIRCUITPY_BLEIO = 0
CIRCUITPY_GAMEPAD = 0
CIRCUITPY_DISPLAYIO = 0
CIRCUITPY_NETWORK = 0
CIRCUITPY_PS2IO = 0
CIRCUITPY_MYMODULE = 0
CIRCUITPY_ULAB = 1
CIRCUITPY_CONTROLLER = 0

FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_BusDevice
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_NeoPixel
