# Makefile for building test.cpp with Vimba SDK

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -L/opt/Vimba_6_1/VimbaCPP/DynamicLib/arm_64bit -lVimbaCPP -llgpio -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lgpiod

# Include directory for VimbaCPP
INCLUDES = -I/opt/Vimba_6_1 -I/usr/include/opencv4

# Directories
BUILD_DIR = ../build
TARGET_DIR = ../bin

# Target executable
TARGET = $(TARGET_DIR)/vimba_piv

# Source files
SOURCES = main.cpp stdafx.cpp signal.cpp

# Object files
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Default rule
all: $(TARGET)

# Rule to create object files
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to link the program
$(TARGET): $(OBJECTS)
	mkdir -p $(TARGET_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Rule for cleaning
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
