#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>

#include "stdafx.h"


// Function to initialize the Vimba API
void StartUpVimba() {
    VimbaSystem& system = VimbaSystem::GetInstance();
    if (VmbErrorSuccess != system.Startup()) {
        throw runtime_error("Failed to start up Vimba API");
    }
}


// Function to shut down the Vimba API
void ShutDownVimba()
{
    VimbaSystem::GetInstance().Shutdown();
}


// Function to create a directory for saving images
std::string CreateImageDirectory()
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_c), "%Y-%m-%d_%H-%M-%S");
    string directoryName = ss.str();
    directoryName = "../data/" + directoryName;
    string command = "mkdir " + directoryName;
    system(command.c_str());
    return directoryName;
}

void WriteBMPHeader(std::ofstream& file, int width, int height, int bitDepth)
{
    BMPHeader header;
    int bytesPerPixel = bitDepth / 8;
    int rowSize = (width * bytesPerPixel + 3) & ~3;
    int dataSize = rowSize * height;

    header.size = 54 + dataSize;
    header.reserved = 0;
    header.offset = 54;
    header.headerSize = 40;
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bitCount = bitDepth;
    header.compression = 0;
    header.imageSize = dataSize;
    header.xPelsPerMeter = 0;
    header.yPelsPerMeter = 0;
    header.clrUsed = 0;
    header.clrImportant = 0;

    file.write("BM", 2);
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
}


// Function to print the current time followed by a specific message.
void PrintCurrentTimeAndMessage(string message) {
    // Obtain the current time
    time_t now = time(0);
    // Convert it to tm struct
    tm *ltm = localtime(&now);

    // Output the time and the message
    std::cout << ltm->tm_hour << ":";
    std::cout << ltm->tm_min << " ";
    std::cout << message << std::endl;
}

// Function to read configuration values from a file named "config.ini".
unordered_map<string, string> readConfigFile() {
    unordered_map<string, string> configValues;
    ifstream configFile("./config.ini");
    string line;

    if (!configFile.is_open()) {
        cerr << "Unable to open config.ini" << endl;
        return configValues;
    }

    while (getline(configFile, line)) { 
        size_t delimiterPos = line.find("=");
        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            key.erase(key.find_last_not_of(" \n\r\t") + 1);
            value.erase(0, value.find_first_not_of(" \n\r\t"));
            configValues[key] = value;
        }
    }
    configFile.close();
    return configValues;
}


// Function to measure the execution time of a code block
template<typename Func>
long long measureExecutionTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now(); // Start time

    f(); // Execute the code block

    auto end = std::chrono::high_resolution_clock::now(); // End time

    // Calculate and return the elapsed time in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return duration.count();
}

/* void SaveFrameToBMP(const FramePtr& frame, const std::string& filename)
{
    if (!frame)
    {
        std::cerr << "Frame is null." << std::endl;
        return;
    }

    VmbUchar_t* buffer = nullptr;
    VmbErrorType res = frame->GetBuffer(buffer);
    if (res != VmbErrorSuccess || buffer == nullptr)
    {
        std::cerr << "Failed to get buffer from frame or buffer is null." << std::endl;
        return;
    }

    VmbUint32_t width = 0, height = 0, bufferSize = 0;
    frame->GetWidth(width);
    // std::cout << "Image Width: " << width << " pixels" << std::endl;

    frame->GetHeight(height);
    // std::cout << "Image Height: " << height << " pixels" << std::endl;

    frame->GetImageSize(bufferSize);

    AVTBitmap bitmap;
    bitmap.buffer = buffer;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.bufferSize = bufferSize;
    bitmap.colorCode = ColorCodeMono16;

    // std::cout << "Creating bitmap..." << std::endl;
    if (!AVTCreateBitmap(&bitmap, buffer))
    {
        std::cerr << "Failed to create bitmap." << std::endl;
        return;
    }

    // std::cout << "Writing bitmap to file..." << std::endl;
    if (!AVTWriteBitmapToFile(&bitmap, filename.c_str()))
    {
        std::cerr << "Failed to write bitmap to file." << std::endl;
    }
    else
    {
        std::cout << "Bitmap written to file successfully." << std::endl;
    }

    //std::cout << "Releasing bitmap..." << std::endl;
    AVTReleaseBitmap(&bitmap);
} */
