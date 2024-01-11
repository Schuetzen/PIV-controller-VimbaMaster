#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <VimbaCPP/Include/VimbaCPP.h>
#include "Bitmap.h"
#include "CosFun.h"


using namespace AVT::VmbAPI;
using namespace std;


// Function to set camera parameters
std::unordered_map<std::string, std::string> SetCameraParameters(CameraPtr camera)
{
    // Before task -> read config.ini and make sure the signal has open
    std::unordered_map<std::string, std::string> configValues = readConfigFile();

    // GetValue 
    double exposure = std::stoi(configValues["exposure_time_in_ms"]);
    int height = std::stoi(configValues["Height"]);
    int width = std::stoi(configValues["Width"]);
    double frequency = std::stoi(configValues["Frequency"]);
    exposure *= 1000;
    FeaturePtr feature;
    VmbErrorType res;
    std::string featureName;

/*     res = camera->GetFeatureByName("TriggerMode", feature);
    if (VmbErrorSuccess == res)
    {
        feature->SetValue("Off");
        feature->GetName(featureName);
        cout << "Feature " << featureName << " set to Off." << endl;
    }
    else {
        cout << "Failed to set Trigger Mode." << endl;
    }

    res = feature->SetValue(frequency);
        if (VmbErrorSuccess != res) {
            std::cout << "Failed to set 'AcquisitionFrameRate' to " << frequency << ". Error code: " << res << std::endl;
            return std::unordered_map<std::string, std::string>{{"error", "Failed to set 'AcquisitionFrameRate'"}};
        } */



    // Access and set the trigger mode to fixed rate
    res = camera->GetFeatureByName("TriggerMode", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue("On"); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"TriggerMode Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            cout<<"error"<<res<<endl;
        }
    }

   res = camera->GetFeatureByName("TriggerSource", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue("Freerun"); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"TriggerSource Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            std::cout<<"TriggerSource Return CODE: "<<res<<std::endl;
        }
    }else{
        cout<<"TriggerSource setting Error: "<<res<<endl;
    }

/*
    // Set 
    res = camera->GetFeatureByName("AquisitionMode", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue("Continuous"); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"Mode Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            cout<<"AquisitionMode："<<res<<endl;
        }
    } */

    /*Fixed Frame Rate*/
    // STEP1. AcquisitionFrameRateEnable needs to be false for camera to determine max possible AcquisitionFrameRate
    res = camera->GetFeatureByName("AcquisitionFrameRateEnable", feature);
    if (VmbErrorSuccess != res) {
        // Handle error
        std::cout << "Error in getting AcquisitionFrameRateEnable feature: " << res << std::endl;
    }    
    cout<<"Acquisition start"<<endl;
    cout<<"error is "<<res<<endl;
    
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(false);
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"FrameRate Set Failed"<<std::endl;
        }else{
            cout<<"Enable Error: "<<res<<endl;
        }
    }else{
        cout<<"Don't find ERROR: " <<res<<endl;
    }
    // STEP2. Read AcquisitionFrameRate
    res = camera->GetFeatureByName("AcquisitionFrameRateAbs", feature);
    cout<<"Rate is "<<res<<endl;
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(22.0);
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"FrameRate Set Failed"<<std::endl;
        }else{
            cout<<"AcquisitionFrameRateAbs Error: "<<res<<endl;
        }
    }
    // STEP3. To fix camera frame rate set AcquisitionFrameRateEnable = true. This makes AcquisitionFrameRate writable
    res = camera->GetFeatureByName("AcquisitionFrameRateEnable", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(true);
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"FrameRate Set Failed"<<std::endl;
        }else{
            cout<<"Enable Error: "<<res<<endl;
        }
    }

    // STEP4. To fix camera frame rate set AcquisitionFrameRateEnable = true. This makes AcquisitionFrameRate writable
    res = camera->GetFeatureByName("AcquisitionFrameRateAbs", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(10.0);
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"FrameRate Set Failed"<<std::endl;
        }else{
            cout<<"AcquisitionFrameRateAbs Error: "<<res<<endl;
        }
    }


    // Set Image Height
    res = camera->GetFeatureByName("Height", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(height); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"Height Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            cout<<"error"<<res<<endl;
        }
    }
    // Set Image Width
    res = camera->GetFeatureByName("Width", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(width); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"Width Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            cout<<"error"<<res<<endl;
        }
    }

    // Set exposure time
    res = camera->GetFeatureByName("PixelFormat", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue("Mono8"); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"PixelFormat Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            cout<<"error"<<res<<endl;
        }
    }

/*     // Center the image in the camera's field of view
    if (VmbErrorSuccess == camera->GetFeatureByName("CenterX", feature))
    {
        //feature->SetValue(true);
    }
    if (VmbErrorSuccess == camera->GetFeatureByName("CenterY", feature))
    {
        //feature->SetValue(true);
    } */

    // Add other necessary parameters

    // Set exposure time
    res = camera->GetFeatureByName("ExposureTimeAbs", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue(exposure); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"ExposureTime Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            cout<<"Exoposure error: "<<res<<endl;
        }
    }else{
        cout<<"Exposure time Setting Error"<<res<<endl;
    }

    cout<<"Setting Successfully"<<endl;

    res = camera->GetFeatureByName("TriggerSource", feature);
    if (VmbErrorSuccess == res) {
        res = feature->SetValue("FixedRate"); // The exact value string may vary
        if (VmbErrorSuccess != res) {
            // Handle error
            std::cout<<"TriggerSource Set Failed, ERROR CODE: "<<res<<std::endl;
        }else{
            std::cout<<"TriggerSource Return CODE: "<<res<<std::endl;
        }
    }else{
        cout<<"TriggerSource setting Error: "<<res<<endl;
    }

    // Return the configuration values
    return configValues;
}


void* VimbaGrab(void* arg){
    // Unpack the arguments
    auto args = static_cast<std::tuple<CameraPtr, int, int, int, int, int>*>(arg);
    CameraPtr camera = std::get<0>(*args);
    //int dt = std::get<1>(*args);
    //int freq = std::get<2>(*args);
    int num_to_save = std::get<1>(*args);

    // Set Camera parameter
    auto configValues = SetCameraParameters(camera);
        
/*      int dt = std::stoi(configValues["dt_in_ms"]);
        int freq = std::stoi(configValues["Frequency"]);
        int dur = std::stoi(configValues["Duration_in_sec"]);
        int height = std::stoi(configValues["Height"]);
        int width = std::stoi(configValues["Width"]); */

    //int num_to_save = 15;

    string directoryName = CreateImageDirectory();
    std::cout<<"Data saved in the "<< directoryName << std::endl;

    
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

    // Close camera and shutdown Vimba at the end of the function
    camera->Close();
    ShutDownVimba();

    return nullptr;

}



// Function to send a software trigger to the camera
/* void SendSoftwareTrigger(const CameraPtr& camera) {
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
} */


void* Signal(void* arg){

    // Unpack the arguments
    auto args = static_cast<std::tuple<CameraPtr, int, int, int, int, int>*>(arg);
    CameraPtr camera = std::get<0>(*args);
    //int exposure = std::get<1>(*args);
    //int dur = std::get<2>(*args);
    //int freq = std::get<3>(*args);
    //int dt = std::get<4>(*args);
    //int laser_on = std::get<5>(*args);
    //int ext = std::get<6>(*args);
        // Example: Send a trigger every second
/*     for (int i = 0; i < 11; ++i) {
        SendSoftwareTrigger(camera);
        this_thread::sleep_for(chrono::seconds(2));
        std::cout<<"Trigger"<<i<<"times"<<std::endl;
    } */

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
    int num_frames = std::stoi(configValues["num_frames"]);

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
    auto grabArgs = make_tuple(camera, exposure, num_frames, freq, height, width, dt);

    // Create and start threads
    pthread_t threadA, threadB;
    pthread_create(&threadA, nullptr, Signal, &signalArgs);
    pthread_create(&threadB, nullptr, VimbaGrab, &grabArgs);

    // Wait for threads to finish
    pthread_join(threadA, nullptr);
    pthread_join(threadB, nullptr);

    return 0;
}

