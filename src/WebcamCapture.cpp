#include "../dependencies.h"

WebcamCapture::WebcamCapture(int width, int height)
{
    devices = setupESCAPI();

    if (devices == 0)
    {
        std::cout<<"ESCAPI initialization failure or no devices found.\n";
        return;
    }

    capture.mWidth = width;
    capture.mHeight = height;
    capture.mTargetBuf = new int[width * height];

    pixels.resize(width*height);

    if (initCapture(0, &capture) == 0)
    {
        std::cout<<"Capture failed - device may already be in use.\n";
        return;
    }

    deviceName = new char[100];

    getCaptureDeviceName(0, deviceName, 100);
}

WebcamCapture::~WebcamCapture()
{
    deinitCapture(0);
}

void WebcamCapture::captureFrame(int deviceID)
{       
    doCapture(0);
    
    while (isCaptureDone(0) == 0)
    {
        /* Wait until capture is done. */
    }
    
    pixels.resize(capture.mWidth*capture.mHeight);
    
    for(int y=0; y<capture.mHeight; y++)
    {
        unsigned char r, g, b;
        
        for(int x=0; x<capture.mWidth; x++)
        {
            b = capture.mTargetBuf[y*capture.mWidth+x];
            g = capture.mTargetBuf[y*capture.mWidth+x] >> 8;
            r = capture.mTargetBuf[y*capture.mWidth+x] >> 16;
            
            pixels[y*capture.mWidth+x].r = static_cast<float>(r) / 255.0f;
            pixels[y*capture.mWidth+x].g = static_cast<float>(g) / 255.0f;
            pixels[y*capture.mWidth+x].b = static_cast<float>(b) / 255.0f;
        }
    }
}