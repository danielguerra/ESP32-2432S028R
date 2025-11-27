# ESP32-2432S028R

Configuration and diagnostic tools for the ESP32-2432S028R development board (also known as "Cheap Yellow Display").

## Hardware

The ESP32-2432S028R is an ESP32-based development board featuring:
- ESP32 microcontroller
- 2.8" ILI9341 TFT display (240x320)
- Resistive touch screen
- Built-in USB-to-serial converter (CH341)

## Contents

### User_Setup.h
TFT_eSPI library configuration file for the ESP32-2432S028R board.

**Installation:**
1. Install the [TFT_eSPI library](https://github.com/Bodmer/TFT_eSPI) in Arduino IDE
2. Copy `User_Setup.h` to your `Arduino/libraries/TFT_eSPI` folder
3. This will configure the library with the correct pins and settings for this board

**Pin Configuration:**
- TFT_MOSI: 13
- TFT_MISO: 12
- TFT_SCLK: 14
- TFT_CS: 15
- TFT_DC: 2
- TFT_BL: 21 (Backlight)
- TOUCH_CS: 33

### spi_miso_diagnostic
Arduino sketch for diagnosing SPI communication with the touch controller.

**Features:**
- Tests SPI communication on MISO line
- Reads X and Y position data from touch controller
- Monitors touch IRQ pin state
- Useful for debugging touch screen issues

**Usage:**
1. Open `spi_miso_diagnostic.ino` in Arduino IDE
2. Upload to your ESP32-2432S028R board
3. Open Serial Monitor at 115200 baud
4. Touch the screen to see SPI communication data

### CH341SER_MAC.ZIP
macOS driver for the CH341 USB-to-serial chip used on this board.

**Installation:**
1. Extract the ZIP file
2. Install the driver package
3. Restart your Mac if prompted
4. The board should now appear as a serial port when connected via USB

## Getting Started

1. Install the CH341 driver (macOS users)
2. Install Arduino IDE and the ESP32 board support
3. Install the TFT_eSPI library
4. Copy `User_Setup.h` to configure TFT_eSPI
5. Upload example sketches or the diagnostic tool

## Resources

- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

## License

This project is provided as-is for educational and development purposes.
