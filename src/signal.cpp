// signal.cpp

#include "signal.h"
#include <iostream>
#include <unistd.h> // For sleep function
#include <gpiod.h>  // Assuming you have a library for GPIO control
#include <chrono>
#include <thread>
#include "stdafx.h"

const char* chipName = "gpiochip4"; // Adjust as needed
unsigned int cameraLine = 17;          // Adjust as needed
unsigned int laserLine = 27;

const int FLASH_DURATION = 1; // Duration of the flash signal in milliseconds

bool Signal() {

    auto config = readConfigFile();
    unordered_map<string, string> configValues = readConfigFile();
    
    float exposure = stof(configValues["exposure_time_in_ms"]);
    float dur = stof(configValues["duration_in_sec"]);
    int freq = stoi(configValues["frequency_in_Hz"]);
    float dt = stof(configValues["delta_t_in_ms"]);
    float laser_on = stof(configValues["laser_on_in_ms"]);
    int ext = stoi(configValues["extension_in_sec"]);

    int flash = FLASH_DURATION;

    int laser_flash = laser_on - flash;
    int exp_flash = exposure-flash - laser_on;
    int loop = 1000/freq-flash-dt-exposure;

    // Open the GPIO chip
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipName);
    if (!chip) {
        std::cerr << "Cannot open GPIO chip" << std::endl;
        return false;
    }

    // Get the GPIO Camera line
    struct gpiod_line *Camline = gpiod_chip_get_line(chip, cameraLine);
    if (!Camline) {
        std::cerr << "Cannot get GPIO line" << std::endl;
        gpiod_chip_close(chip);
        return false;
    }
    // Get the GPIO Laser line
    struct gpiod_line *Laserline = gpiod_chip_get_line(chip, laserLine);
    if (!Laserline) {
        std::cerr << "Cannot get GPIO line" << std::endl;
        gpiod_chip_close(chip);
        return false;
    }

    // Request the Cam line for output
    if (gpiod_line_request_output(Camline, "gpio_control", 0) < 0) {
        std::cerr << "Cannot request line for output" << std::endl;
        gpiod_line_release(Camline);
        gpiod_chip_close(chip);
        return false;
    }
    // Request the Laser line for output
    if (gpiod_line_request_output(Laserline, "gpio_control", 0) < 0) {
        std::cerr << "Cannot request line for output" << std::endl;
        gpiod_line_release(Laserline);
        gpiod_chip_close(chip);
        return false;
    }

    PrintCurrentTimeAndMessage("Signal Start");
    // Blink an LED attached to the line
    auto start_time = std::chrono::steady_clock::now();
    while(true) {
        // exit loop after dur seconds
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (elapsed_time >= dur + ext) {
            break;
        }
        
        // Turn camera on
        gpiod_line_set_value(Camline, 1);
        std::this_thread::sleep_for(std::chrono::microseconds(flash * 1000));

        // Turn camera off
        gpiod_line_set_value(Camline, 0);
        std::this_thread::sleep_for(std::chrono::microseconds(exp_flash * 1000));

        // Turn laser on
        gpiod_line_set_value(Laserline, 1);
        std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long>(dt * 1000)));

        // Turn camera on again
        gpiod_line_set_value(Camline, 1);
        std::this_thread::sleep_for(std::chrono::microseconds(flash * 1000));

        // Turn camera off again
        gpiod_line_set_value(Camline, 0);
        std::this_thread::sleep_for(std::chrono::microseconds(laser_flash * 1000));

        // Turn laser off
        gpiod_line_set_value(Laserline, 0);
        std::this_thread::sleep_for(std::chrono::microseconds(loop * 1000));
// wait until the next pair
    }


    // Release line and close chip
    gpiod_line_release(Laserline);
    gpiod_line_release(Camline);

    gpiod_chip_close(chip);

    return true;
}
