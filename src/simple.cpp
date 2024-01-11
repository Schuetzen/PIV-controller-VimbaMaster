#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <unordered_map>
#include <pigpio.h>
#include <VimbaCPP/Include/VimbaCPP.h>

using namespace AVT::VmbAPI;
using namespace std;

// Function declarations
unordered_map<string, string> readConfigFile();
void StartUpVimba();
void ShutDownVimba();
unordered_map<string, string> SetCameraParameters(CameraPtr camera);
string CreateImageDirectory();
void SaveFrameToBMP(const FramePtr& frame, const string& filename);
int MultiGrab();
void PrintCurrentTimeAndMessage(string message);
void* Signal(void* arg);

// Main function
int main()
{
    MultiGrab();
    return 0;
}


