#include <iostream>
#include <gpiod.h>
#include <unistd.h> // for sleep

bool Signal(const char* chipName, unsigned int lineNum) {
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



int main() {
    const char* chipName = "gpiochip4"; // adjust as needed
    unsigned int lineNum = 17;          // adjust as needed

    if (!Signal(chipName, lineNum)) {
        std::cerr << "Failed to control GPIO line" << std::endl;
        return 1;
    }

    return 0;
}

