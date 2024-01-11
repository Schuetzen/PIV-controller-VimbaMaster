# Makefile for building test.cpp with Vimba SDK

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -L/opt/Vimba_6_1/VimbaCPP/DynamicLib/arm_64bit -lVimbaCPP -llgpio

# Include directory for VimbaCPP
INCLUDES = -I/opt/Vimba_6_1 

# Target executable
TARGET = piv

# Source files
SOURCES = PivProgram.cpp Bitmap.cpp

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
