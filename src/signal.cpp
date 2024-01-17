// signal.cpp

#include "signal.h"
#include <iostream>
#include <unistd.h> // For sleep function
#include <gpiod.h>  // Assuming you have a library for GPIO control

const char* chipName = "gpiochip4"; // Adjust as needed
unsigned int lineNum = 17;          // Adjust as needed

bool Signal() {
    // Open the GPIO chip
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipName);
    if (!chip) {
        std::cerr << "Cannot open GPIO chip" << std::endl;
        return false;
    }

    // Get the GPIO line
    struct gpiod_line *line = gpiod_chip_get_line(chip, lineNum);
    if (!line) {
        std::cerr << "Cannot get GPIO line" << std::endl;
        gpiod_chip_close(chip);
        return false;
    }

    // Request the line for output
    if (gpiod_line_request_output(line, "gpio_control", 0) < 0) {
        std::cerr << "Cannot request line for output" << std::endl;
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return false;
    }

    // Blink an LED attached to the line
    for (int i = 0; i < 1000; ++i) {
        gpiod_line_set_value(line, 1); // Set line high
        sleep(1);
        gpiod_line_set_value(line, 0); // Set line low
        sleep(1);
    }

    // Release line and close chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return true;
}
