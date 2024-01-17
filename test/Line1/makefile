# Makefile for building test.cpp with Vimba SDK

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -L/opt/Vimba_6_1/VimbaCPP/DynamicLib/arm_64bit -lVimbaCPP -llgpio -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

# Include directory for VimbaCPP
INCLUDES = -I/opt/Vimba_6_1 -I/usr/include/opencv4

# Target executable
TARGET = line1

# Source files
SOURCES = main.cpp stdafx.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Rule to create object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to link the program
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Rule for cleaning
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: clean
