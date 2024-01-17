// standard application framework extensions
#ifndef STDAFXH
#define STDAFXH

#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <VimbaCPP/Include/VimbaCPP.h>



using namespace AVT::VmbAPI;
using namespace std;


struct BMPHeader {
    uint32_t size;        
    uint32_t reserved;    
    uint32_t offset;      
    uint32_t headerSize;  
    uint32_t width;       
    uint32_t height;      
    uint16_t planes;      
    uint16_t bitCount;    
    uint32_t compression; 
    uint32_t imageSize;   
    uint32_t xPelsPerMeter;
    uint32_t yPelsPerMeter;
    uint32_t clrUsed;     
    uint32_t clrImportant;
};

void StartUpVimba();
void ShutDownVimba();
void WriteBMPHeader(std::ofstream& file, int width, int height, int bitDepth);
std::string CreateImageDirectory();
void SaveFrameToBMP(const FramePtr& frame, const std::string& filename);
void PrintCurrentTimeAndMessage(std::string message);
unordered_map<string, string> readConfigFile();

template<typename Func>
long long measureExecutionTime(Func f);
#endif
