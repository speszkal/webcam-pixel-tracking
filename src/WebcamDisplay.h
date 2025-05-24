#pragma once

class WebcamDisplay {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        int width;
        int height;
        bool initialized;
    
    public:
        WebcamDisplay();
        ~WebcamDisplay();
        
        // Initialize with window dimensions
        bool initialize(const char* title, int width, int height);
        
        // Display the frame from the color vector
        bool displayFrame(const std::vector<Color>* pixelData);
        
        // Clean up resources
        void close();
};