#pragma once

class WebcamCapture
{
    public:
        WebcamCapture(int width, int height);
        ~WebcamCapture();

        char* getDeviceName() { return deviceName; };
        void captureFrame(int deviceID);

        std::vector<Color>* getFrame() { return &pixels; };

    private:
        int devices;
        SimpleCapParams capture;
        std::vector<Color> pixels;
        char* deviceName;
};