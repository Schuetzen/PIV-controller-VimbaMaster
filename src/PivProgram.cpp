#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <lgpio.h>
#include <thread>

// Special Out put
#include "Bitmap.h"
// Vimba Include
#include <VimbaCPP/Include/VimbaCPP.h>


using namespace AVT::VmbAPI;
using namespace std;


std::unordered_map<std::string, std::string> readConfigFile() {}

// Function to initialize the Vimba API
void StartUpVimba()
{
    VimbaSystem& system = VimbaSystem::GetInstance();
    if (VmbErrorSuccess != system.Startup())
    {
        throw runtime_error("Failed to start up Vimba API");
    }
}

// Function to shut down the Vimba API
void ShutDownVimba()
{
    VimbaSystem::GetInstance().Shutdown();
}

// Function to set camera parameters
std::unordered_map<std::string, std::string> SetCameraParameters(CameraPtr camera)
{
    // Before task -> read config.ini and make sure the signal has open
    std::unordered_map<std::string, std::string> configValues = readConfigFile();

    // GetValue 
    int exposure = std::stoi(configValues["exposure_time_in_ms"]);
    int height = std::stoi(configValues["Height"]);
    int width = std::stoi(configValues["Width"]);
    int laser_on = std::stoi(configValues["laser_on"]);
    int extension_time = std::stoi(configValues["extension_time_in_sec"]);

    FeaturePtr feature;
    VmbErrorType res;
    std::string featureName;


     // Set hardware trigger
/*     if (VmbErrorSuccess == camera->GetFeatureByName("TriggerMode", feature))
    {
        feature->SetValue("On");
    }
    if (VmbErrorSuccess == camera->GetFeatureByName("TriggerSelector", feature))
    {
        feature->SetValue("FrameStart");
    }
    if (VmbErrorSuccess == camera->GetFeatureByName("TriggerSource", feature))
    {
        feature->SetValue("Line3");
    } */

    res = camera->GetFeatureByName("TriggerMode", feature);
    if (VmbErrorSuccess == res)
    {
        feature->SetValue("On");
        feature->GetName(featureName);
        cout << "Feature " << featureName << " set to On." << endl;
    }
    else {
        cout << "Failed to set Trigger Mode." << endl;
    }

    // Print the values after setting them
    std::string value;
    feature->GetValue(value);
    feature->GetName(featureName);
    cout << "Feature " << featureName << " set to " << value << endl;

    res = camera->GetFeatureByName("TriggerSelector", feature);
    if (VmbErrorSuccess == res)
    {
        feature->SetValue("FrameStart");
    }
    else {
        cout << "Failed to set Trigger Selector." << endl;
    }

    res = camera->GetFeatureByName("TriggerSource", feature);
    if (VmbErrorSuccess == res)
    {
        feature->SetValue("Software");
        cout << "Trigger Source set to Software." << endl;
    }
    else {
        cout << "Failed to set Trigger Source." << endl;
    }

    // Set the height and width of the camera image
    if (VmbErrorSuccess == camera->GetFeatureByName("Height", feature))
    {
        feature->SetValue(height);
        //std::cout << "Image Height: " << height << " pixels" << std::endl;
    }
    if (VmbErrorSuccess == camera->GetFeatureByName("Width", feature))
    {
        feature->SetValue(width);
        //std::cout << "Image Width: " << width << " pixels" << std::endl;

    }

    // Center the image in the camera's field of view
    if (VmbErrorSuccess == camera->GetFeatureByName("CenterX", feature))
    {
        //feature->SetValue(true);
    }
    if (VmbErrorSuccess == camera->GetFeatureByName("CenterY", feature))
    {
        //feature->SetValue(true);
    }

    // Set exposure time
    if (VmbErrorSuccess == camera->GetFeatureByName("ExposureTimeAbs", feature))
    {
        feature->SetValue(exposure);
    }

    // Add other necessary parameters
    cout<<"Setting Successfully"<<endl;




    // Return the configuration values
    return configValues;
}

// Function to create a directory for saving images
string CreateImageDirectory()
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_c), "%Y-%m-%d_%H-%M-%S");
    string directoryName = ss.str();
    string command = "mkdir " + directoryName;
    system(command.c_str());
    return directoryName;
}

// Function to save a frame to a file in raw format
struct BMPHeader
{
    uint32_t size;        // Size of the BMP file
    uint32_t reserved;    // Reserved; must be zero
    uint32_t offset;      // Offset to the array of pixel data relative to the start of the file
    uint32_t headerSize;  // Size of the BITMAPINFOHEADER structure, in bytes
    uint32_t width;       // Width of the image, in pixels
    uint32_t height;      // Height of the image, in pixels
    uint16_t planes;      // Number of planes for the target device, must be set to 1
    uint16_t bitCount;    // Number of bits per pixel
    uint32_t compression; // Type of compression
    uint32_t imageSize;   // Size of the image data
    uint32_t xPelsPerMeter; // Horizontal resolution in pixels per meter (unused)
    uint32_t yPelsPerMeter; // Vertical resolution in pixels per meter (unused)
    uint32_t clrUsed;     // Number of colors used in the bitmap (0 to default to 2^n)
    uint32_t clrImportant; // Number of important colors (0 when every color is important)
};

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

void SaveFrameToBMP(const FramePtr& frame, const std::string& filename)
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
    std::cout << "Image Width: " << width << " pixels" << std::endl;

    frame->GetHeight(height);
    std::cout << "Image Height: " << height << " pixels" << std::endl;

    frame->GetImageSize(bufferSize);

    AVTBitmap bitmap;
    bitmap.buffer = buffer;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.bufferSize = bufferSize;
    bitmap.colorCode = ColorCodeMono8;

    std::cout << "Creating bitmap..." << std::endl;
    if (!AVTCreateBitmap(&bitmap, buffer))
    {
        std::cerr << "Failed to create bitmap." << std::endl;
        return;
    }

    std::cout << "Writing bitmap to file..." << std::endl;
    if (!AVTWriteBitmapToFile(&bitmap, filename.c_str()))
    {
        std::cerr << "Failed to write bitmap to file." << std::endl;
    }
    else
    {
        std::cout << "Bitmap written to file successfully." << std::endl;
    }

    std::cout << "Releasing bitmap..." << std::endl;
    AVTReleaseBitmap(&bitmap);
}

void* VimbaGrab(void* arg){
    // Unpack the arguments
    auto args = static_cast<std::tuple<CameraPtr, int, int, int, int, int>*>(arg);
    CameraPtr camera = std::get<0>(*args);
    int dt = std::get<1>(*args);
    int freq = std::get<2>(*args);
    int dur = std::get<2>(*args);

    // Set Camera parameter
    auto configValues = SetCameraParameters(camera);
        
/*         int dt = std::stoi(configValues["dt_in_ms"]);
        int freq = std::stoi(configValues["Frequency"]);
        int dur = std::stoi(configValues["Duration_in_sec"]);
        int height = std::stoi(configValues["Height"]);
        int width = std::stoi(configValues["Width"]); */

        int num_to_save = 10;

        string directoryName = CreateImageDirectory();
        std::cout<<"Data saved in the "<< directoryName << std::endl;
        camera->StartContinuousImageAcquisition(200, IFrameObserverPtr(), FrameAllocation_AnnounceFrame);

        cout<<"Start Capture Frames"<<endl;
        
        for (int i = 0; i < num_to_save; ++i)
        {
            FramePtr frame;
            VmbErrorType result = camera->AcquireSingleImage(frame, 2000);
            if (VmbErrorSuccess == result)
            {
                // cout<<"Get Image "<< i << " Successfully"<<endl;
                string filename = directoryName + "/image" + to_string(i) + ".bmp";
                SaveFrameToBMP(frame, filename);

            }
            else
            {
                cout << "Failed to acquire image." << endl;
            }
        }

    // Properly close camera and shutdown Vimba at the end of the function
    camera->StopContinuousImageAcquisition();
    camera->Close();
    ShutDownVimba();

        return nullptr;

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

// Function to send a software trigger to the camera
void SendSoftwareTrigger(const CameraPtr& camera) {
    FeaturePtr feature;
    VmbErrorType res = camera->GetFeatureByName("TriggerSoftware", feature);
    if (VmbErrorSuccess == res) {
        res = feature->RunCommand();
        if (VmbErrorSuccess != res) {
            cerr << "Failed to send software trigger." << endl;
        }
    } else {
        cerr << "TriggerSoftware feature not found." << endl;
    }
}

void* Signal(void* arg){

    // Unpack the arguments
    auto args = static_cast<std::tuple<CameraPtr, int, int, int, int, int>*>(arg);
    CameraPtr camera = std::get<0>(*args);
    //int exposure = std::get<1>(*args);
    int dur = std::get<2>(*args);
    //int freq = std::get<3>(*args);
    //int dt = std::get<4>(*args);
    //int laser_on = std::get<5>(*args);
    //int ext = std::get<6>(*args);
        // Example: Send a trigger every second
    for (int i = 0; i < 11; ++i) {
        SendSoftwareTrigger(camera);
        this_thread::sleep_for(chrono::seconds(2));
        std::cout<<"Trigger"<<i<<"times"<<std::endl;
    }

/*     int flash = 1;

    int handle;
    handle = lgGpiochipOpen(0); // Open GPIO chip
    if (handle < 0) {
        // Initialization failed
        return nullptr;
    }

    // GPIO pin 17 = physical pin 11
    int cameraPin = 17;    
    // GPIO pin 27 = physical pin 13 
    //int laserPin = 27;

    lgGpioClaimOutput(handle, 0, cameraPin, 0); */ // Set camera signal pin mode
    //lgGpioClaimOutput(handle, 0, laserPin, 0);  // Set laser signal pin mode

/*     float laser_flash = laser_on - flash;
    float exp_flash = exposure - flash - laser_on;
    float loop = 1000.0/freq - flash - dt - exposure;

    std::cout << "Laser is running!" << std::endl;    
    auto start_time = std::chrono::steady_clock::now(); */
/*     while(true) {
        // exit loop after dur seconds
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (elapsed_time >= dur + ext) {
            break;
        }
        SendSoftwareTrigger(camera); */

/*         lgGpioWrite(handle, cameraPin, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(flash));
        lgGpioWrite(handle, cameraPin, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(exp_flash)));
        lgGpioWrite(handle, laserPin, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(dt));

        lgGpioWrite(handle, cameraPin, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(flash));
        lgGpioWrite(handle, cameraPin, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(laser_flash)));
        lgGpioWrite(handle, laserPin, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(loop))); // wait until the next pair 
    } */

    //lgGpiochipClose(handle); // Clean up lgpio

    return nullptr;
}



// Main function
int main()
{
    // Before task -> read config.ini and make sure the signal has open
    std::unordered_map<std::string, std::string> configValues = readConfigFile();
    // GetValue 
    int exposure = std::stoi(configValues["exposure_time_in_ms"]);
    int dt = std::stoi(configValues["dt_in_ms"]);
    int freq = std::stoi(configValues["Frequency"]);
    int dur = std::stoi(configValues["Duration_in_sec"]);
    int height = std::stoi(configValues["Height"]);
    int width = std::stoi(configValues["Width"]);
    int laser_on = std::stoi(configValues["laser_on"]);
    int extension_time = std::stoi(configValues["extension_time_in_sec"]);

    // Initialize Vimba and open the first camera
    StartUpVimba();
    CameraPtrVector cameras;
    VimbaSystem::GetInstance().GetCameras(cameras);
    if (cameras.empty()) {
        cerr << "No cameras found" << endl;
        return 1;
    }

    CameraPtr camera = cameras[0];
    camera->Open(VmbAccessModeFull);

    // Create thread arguments
    auto signalArgs = make_tuple(camera, exposure, dur, freq, dt, laser_on, extension_time);
    auto grabArgs = make_tuple(camera, exposure, dur, freq, height, width, dt);

    // Create and start threads
    pthread_t threadA, threadB;
    pthread_create(&threadA, nullptr, Signal, &signalArgs);
    pthread_create(&threadB, nullptr, VimbaGrab, &grabArgs);

    // Wait for threads to finish
    pthread_join(threadA, nullptr);
    pthread_join(threadB, nullptr);

    return 0;
}

// Function to read configuration values from a file named "config.ini".
std::unordered_map<std::string, std::string> readConfigFile() {
    std::unordered_map<std::string, std::string> configValues;

    // open config.ini
    std::ifstream configFile("./config.ini");
    std::string line;

    if (configFile.is_open()) {
        while (std::getline(configFile, line)) { 
            // Read by lines
            size_t delimiterPos = line.find("=");

            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);

                key.erase(key.find_last_not_of(" \n\r\t") + 1);
                value.erase(0, value.find_first_not_of(" \n\r\t"));

                configValues[key] = value;
            }
        }
        // Close File
        configFile.close();
    } else {
        std::cerr << "Unable to open config.ini" << std::endl;
    }

    return configValues;
}
