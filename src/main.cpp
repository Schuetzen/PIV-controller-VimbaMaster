#include <opencv2/opencv.hpp>
#include <VimbaCPP/Include/VimbaCPP.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include "stdafx.h"
#include "signal.h"

using namespace AVT::VmbAPI;
using namespace cv;
using namespace std;


/*
FrameObserver Class: 
This class is responsible for handling frames received from the camera. It checks if the frame is received correctly, processes the image, and saves it.
*/
class FrameObserver : public IFrameObserver {
public:
    FrameObserver(CameraPtr pCamera, unsigned int numFrames) 
        : IFrameObserver(pCamera), m_frameCounter(0), m_numFrames(numFrames) {
        imageDirectory = CreateImageDirectory();
        cout<<"Image Directory Created"<<endl;
    }

    void FrameReceived(const FramePtr pFrame) {
        VmbFrameStatusType eReceiveStatus;
        // Check if frame received successfully
        if (VmbErrorSuccess == pFrame->GetReceiveStatus(eReceiveStatus) && VmbFrameStatusComplete == eReceiveStatus) {
            VmbUchar_t *pImage;
            if (VmbErrorSuccess == pFrame->GetImage(pImage) && pImage != nullptr) {
                VmbUint32_t width, height;
                pFrame->GetWidth(width);
                pFrame->GetHeight(height);

                // Assuming the pixel format is Mono8
                Mat img(height, width, CV_16UC1, pImage);

                // Save the image
                cout<<"Saving Image ..."<<endl;
                stringstream filename;
                filename << imageDirectory << "/image" << setfill('0') << setw(7) << m_frameCounter << ".tif";
                imwrite(filename.str(), img);

                // Stop acquisition after reaching the desired number of frames
                if (++m_frameCounter >= m_numFrames) {
                    this->m_pCamera->StopContinuousImageAcquisition();
                    cout<<"Image Acquisition Stopped"<<endl;
                }
            } else {
                cerr << "Error: Failed to get image from frame." << endl;
            }
        }

        // Re-queue the frame for continuous acquisition
        m_pCamera->QueueFrame(pFrame);
    }

    unsigned int GetFrameCounter() const {
        return m_frameCounter;
    }

private:
    unsigned int m_frameCounter;
    unsigned int m_numFrames;
    string imageDirectory;
};


/*
SetCameraParameters Function: 
This function sets various camera parameters based on a configuration file.
Error handling should be implemented for each parameter set operation.
*/
unordered_map<string, string> SetCameraParameters(CameraPtr camera)
{
    // Before task -> read config.ini and make sure the signal has open
    unordered_map<string, string> configValues = readConfigFile();
    float exposure = stof(configValues["exposure_time_in_ms"]);
    int height = stoi(configValues["Height"]);
    int width = stoi(configValues["Width"]);
    float framerate = stof(configValues["FrameRate"]);
    exposure *= 1000;
    FeaturePtr feature;
    VmbErrorType err;
    string featureName;
    
    int num_frames = stoi(configValues["num_frames"]);
    configValues["num_frames"] = to_string(num_frames);

    err = camera->GetFeatureByName("TriggerMode", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue("On");
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"TriggerMode Set Failed. Error: "<<err<<endl;
        }else{
            cout<<"TriggerMode Set to:"<<" On "<<endl;
        }
    }else{
        cout<<"TriggerMode Set Failed. Error: "<<err<<endl;
    }

    err = camera->GetFeatureByName("TriggerSource", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue("Line1"); // FixedRate, Line1, Line2, Freerun, software
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"TriggerSource set Failed, Error: "<<err<<endl;
        }else{
            cout<<"TriggerSource Set to: "<<"Fixed Rate"<<endl;
        }
    }else{
        cout<<"TriggerSource set Error: "<<err<<endl;
    }

    // Set the pixel format to Mono8
    err = camera->GetFeatureByName("PixelFormat", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue("Mono12");
    }else {
        cerr << "Could not set pixel format: " << err << endl;
    }

    // STEP4. To fix camera frame rate set AcquisitionFrameRateEnable = true. This makes AcquisitionFrameRate writable
/*     err = camera->GetFeatureByName("AcquisitionFrameRateAbs", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue(framerate);
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"FrameRate Set Failed. Error: "<<err<<endl;
        }else{
            cout<<"AcquisitionFrameRateAbs. Frame Rate (Abs): "<<framerate<<endl;
        }
    }else{
        cout<<"FrameRate Set Failed. Error: "<<err<<endl;
    } */


        // Set Image Height
    err = camera->GetFeatureByName("Height", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue(height); // The exact value string may vary
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"Height Set Failed, ERROR: "<<err<<endl;
        }else{
            cout<<"Height Set successfully. Height: "<<height<<" pix"<<endl;
        }
    }
    // Set Image Width
    err = camera->GetFeatureByName("Width", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue(width); // The exact value string may vary
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"Width Set Failed, ERROR: "<<err<<endl;
        }else{
            cout<<"Height Set successfully. Width: "<<width<<" pix"<<endl;
        }
    }

    // Set exposure time
    err = camera->GetFeatureByName("ExposureTimeAbs", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue(exposure); // The exact value string may vary
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"ExposureTime Set Failed, Error: "<<err<<endl;
        }else{
            cout<<"Exposure time Set Successfully. Exposure time: "<<exposure<<" us"<<endl;        }
    }else{
        cout<<"ExposureTime Set Failed, Error: "<<err<<endl;
    }

    // Return the configuration values
    return configValues;
}

int Grab() {
    VimbaSystem& sys = VimbaSystem::GetInstance();
    
    VmbErrorType err = sys.Startup();
    if (VmbErrorSuccess != err) {
        cerr << "Could not start Vimba system: " << err << endl;
        return -1;
    }else{
        cout<<"Vimba System Started Successfully"<<endl;
    }

    CameraPtrVector cameras;
    err = sys.GetCameras(cameras);
    if (VmbErrorSuccess != err || cameras.empty()) {
        cerr << "No cameras found!" << endl;
        sys.Shutdown();
        return -1;
    }else{
        cout<<"Camera Found Successfully"<<endl;
    }

    CameraPtr camera = cameras[0];
    err = camera->Open(VmbAccessModeFull);
    if (VmbErrorSuccess != err) {
        cerr << "Could not open camera: " << err << endl;
        sys.Shutdown();
        return -1;
    }else{
        cout<<"Camera Opened Successfully"<<endl;
    }

    auto configValues = SetCameraParameters(camera);
    unsigned int num_frames = stoi(configValues["num_frames"]);
    FrameObserver observer(camera, num_frames);

    if (configValues.find("error") != configValues.end()) {
        cerr << "Error setting camera parameters: " << configValues["error"] << endl;
        camera->Close();
        sys.Shutdown();
        return -1;
    }else{
        cout<<"Camera Parameters Set Successfully"<<endl;
    }
    
    err = camera->StartContinuousImageAcquisition(num_frames, IFrameObserverPtr(&observer));
    if (VmbErrorSuccess != err) {
        cerr << "Could not start image acquisition: " << err << endl;
        camera->Close();
        sys.Shutdown();
        return -1;
    }else{
        cout<<"Image Acquisition Started Successfully"<<endl;
    }

    while (observer.GetFrameCounter() < num_frames) {
        cout<<"Waiting for "<<num_frames<<" frames"<<endl;
    }

    camera->Close();
    cout<<"Camera Closed Successfully"<<endl;
    sys.Shutdown();

    return 0;
}

int main(int argc, char* argv[]) {

    cout<<"Grabbing Images"<<endl;
    // Create thread for Grab function
    std::thread grabThread(Grab);

    cout<<"Signaling"<<endl;
    // Create thread for Signal function
    std::thread signalThread(Signal);

    // Wait for threads to finish
    grabThread.join();
    signalThread.join();

    std::cout << "Both Grab and Signal functions have completed." << std::endl;

    return 0;
}

