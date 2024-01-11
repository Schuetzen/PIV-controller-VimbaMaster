#include <opencv2/opencv.hpp>
#include <VimbaCPP/Include/VimbaCPP.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "stdafx.h"

using namespace AVT::VmbAPI;
using namespace cv;
using namespace std;

class FrameObserver : public IFrameObserver {
public:
    FrameObserver(CameraPtr pCamera, unsigned int numFrames) 
        : IFrameObserver(pCamera), m_frameCounter(0), m_numFrames(numFrames) {
        imageDirectory = CreateImageDirectory();
    }

    void FrameReceived(const FramePtr pFrame) {
        VmbFrameStatusType eReceiveStatus;
        if (VmbErrorSuccess == pFrame->GetReceiveStatus(eReceiveStatus) && VmbFrameStatusComplete == eReceiveStatus) {
            VmbUchar_t *pImage;
            VmbErrorType res = pFrame->GetImage(pImage);
            if (VmbErrorSuccess == res) {
                VmbUint32_t width, height;
                pFrame->GetWidth(width);
                pFrame->GetHeight(height);

                // Assuming the pixel format is Mono8
                Mat img(height, width, CV_8UC1, pImage);

                // Save the image
                stringstream filename;
                filename << imageDirectory << "/image" << setfill('0') << setw(7) << m_frameCounter << ".png";
                imwrite(filename.str(), img);

                if (++m_frameCounter >= m_numFrames) {
                    // Stop acquisition after 10 frames
                    this->m_pCamera->StopContinuousImageAcquisition();
                }
            }
        }

        // Re-queue the frame
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

unordered_map<string, string> SetCameraParameters(CameraPtr camera)
{
    // Before task -> read config.ini and make sure the signal has open
    unordered_map<string, string> configValues = readConfigFile();

    // GetValue 
    double exposure = stoi(configValues["exposure_time_in_ms"]);
    int height = stoi(configValues["Height"]);
    int width = stoi(configValues["Width"]);
    double frequency = stoi(configValues["Frequency"]);
    //int num_frames = stoi(configValues["num_frames"]);


    exposure *= 1000;
    FeaturePtr feature;
    VmbErrorType err;
    string featureName;
    
    err = camera->GetFeatureByName("TriggerMode", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue("On"); // The exact value string may vary
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
        err = feature->SetValue("FixedRate"); // The exact value string may vary
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
        err = feature->SetValue("Mono8");
    }else {
        cerr << "Could not set pixel format: " << err << endl;
    }

    // STEP4. To fix camera frame rate set AcquisitionFrameRateEnable = true. This makes AcquisitionFrameRate writable
    err = camera->GetFeatureByName("AcquisitionFrameRateAbs", feature);
    if (VmbErrorSuccess == err) {
        err = feature->SetValue(frequency);
        if (VmbErrorSuccess != err) {
            // Handle error
            cout<<"FrameRate Set Failed. Error: "<<err<<endl;
        }else{
            cout<<"AcquisitionFrameRateAbs. Frame Rate (Abs): "<<frequency<<endl;
        }
    }else{
        cout<<"FrameRate Set Failed. Error: "<<err<<endl;
    }


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
            cout<<"ExposureTime Set Failed, ERROR: "<<err<<endl;
        }else{
            cout<<"Exposure time Set Successfully. Exposure time: "<<exposure<<" us"<<endl;        }
    }else{
        cout<<"ExposureTime Set Failed, ERROR: "<<err<<endl;
    }

    // Add num_frames to the configValues map
    //configValues["num_frames"] = to_string(num_frames);
    // Return the configuration values
    return configValues;
}

int main(int argc, char* argv[]) {
    VimbaSystem& sys = VimbaSystem::GetInstance();
    VmbErrorType err = sys.Startup();
    if (VmbErrorSuccess != err) {
        cerr << "Could not start Vimba system: " << err << endl;
        return -1;
    }

    CameraPtrVector cameras;
    err = sys.GetCameras(cameras);
    if (VmbErrorSuccess != err || cameras.empty()) {
        cerr << "No cameras found!" << endl;
        sys.Shutdown();
        return -1;
    }else{
        cout<<"Success"<<endl;
    }



    CameraPtr camera = cameras[0];

    // Set camera parameters
    auto configValues = SetCameraParameters(camera);

    // Check if there was an error during setting parameters
    if (configValues.find("error") != configValues.end()) {
        cerr << "Error setting camera parameters: " << configValues["error"] << endl;
        camera->Close();
        sys.Shutdown();
        return -1;
    }

    // Read num_frames from configValues
    //unsigned int num_frames = stoi(configValues["num_frames"]);
    int num_frames = 15;

    FrameObserver observer(camera, num_frames);
    err = camera->Open(VmbAccessModeFull);
    if (VmbErrorSuccess != err) {
        cerr << "Could not open camera: " << err << endl;
        sys.Shutdown();
        return -1;
    }





    err = camera->StartContinuousImageAcquisition(10, IFrameObserverPtr(&observer));
    if (VmbErrorSuccess != err) {
        cerr << "Could not start image acquisition: " << err << endl;
        camera->Close();
        sys.Shutdown();
        return -1;
    }

    // Wait until acquisition is done
    while (observer.GetFrameCounter() < num_frames) {
        // Here, you may want to implement a sleep or a more efficient wait
        // to avoid busy waiting, depending on your application's requirements
    }

    if (camera) {
    camera->Close();
    }
    sys.Shutdown();

    return 0;
}
