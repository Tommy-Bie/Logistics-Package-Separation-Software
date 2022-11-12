#ifndef SAMPLE_COMMON_COMMON_HPP_
#define SAMPLE_COMMON_COMMON_HPP_

#include "Utils.hpp"

#include <fstream>
#include <iterator>
#include <opencv2/opencv.hpp>
//#include "TyIsp.h"
//#include "BayerISP.hpp"



enum{
    PC_FILE_FORMAT_XYZ = 0,
};



/* class CallbackWrapper
{
public:
    typedef void(*TY_FRAME_CALLBACK) (TY_FRAME_DATA*, void* userdata);

    CallbackWrapper(){
        _hDevice = NULL;
        _cb = NULL;
        _userdata = NULL;
    }

    TY_STATUS TYRegisterCallback(TY_DEV_HANDLE hDevice, TY_FRAME_CALLBACK v, void* userdata)
    {
        _hDevice = hDevice;
        _cb = v;
        _userdata = userdata;
        _exit = false;
        _cbThread.create(&workerThread, this);
        return TY_STATUS_OK;
    }

    void TYUnregisterCallback()
    {
        _exit = true;
        _cbThread.destroy();
    }

private:
    static void* workerThread(void* userdata)
    {
        CallbackWrapper* pWrapper = (CallbackWrapper*)userdata;
        TY_FRAME_DATA frame;

        while (!pWrapper->_exit)
        {
            int err = TYFetchFrame(pWrapper->_hDevice, &frame, 100);
            if (!err) {
                pWrapper->_cb(&frame, pWrapper->_userdata);
            }
        }
        LOGI("frameCallback exit!");
        return NULL;
    }

    TY_DEV_HANDLE _hDevice;
    TY_FRAME_CALLBACK _cb;
    void* _userdata;

    bool _exit;
    TYThread _cbThread;
};



#ifdef _WIN32
static int get_fps() {
    static int fps_counter = 0;
    static clock_t fps_tm = 0;
   const int kMaxCounter = 250;
   fps_counter++;
   if (fps_counter < kMaxCounter) {
     return -1;
   }
   int elapse = (clock() - fps_tm);
   int v = (int)(((float)fps_counter) / elapse * CLOCKS_PER_SEC);
   fps_tm = clock();

   fps_counter = 0;
   return v;
 }
#else
static int get_fps() {
    static int fps_counter = 0;
    static clock_t fps_tm = 0;
    const int kMaxCounter = 200;
    struct timeval start;
    fps_counter++;
    if (fps_counter < kMaxCounter) {
        return -1;
    }

    gettimeofday(&start, NULL);
    int elapse = start.tv_sec * 1000 + start.tv_usec / 1000 - fps_tm;
    int v = (int)(((float)fps_counter) / elapse * 1000);
    gettimeofday(&start, NULL);
    fps_tm = start.tv_sec * 1000 + start.tv_usec / 1000;

    fps_counter = 0;
    return v;
}
#endif

static std::vector<uint8_t> TYReadBinaryFile(const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()){
        return std::vector<uint8_t>();
    }
    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());

    return vec;
}

 */
#endif