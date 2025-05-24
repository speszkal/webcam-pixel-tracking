// WebcamDisplay.cpp
#include "../dependencies.h"

WebcamDisplay::WebcamDisplay() : 
    window(nullptr), 
    renderer(nullptr), 
    texture(nullptr),
    width(0),
    height(0),
    initialized(false) {}

WebcamDisplay::~WebcamDisplay() {
    close();
}

bool WebcamDisplay::initialize(const char* title, int w, int h) {
    if (initialized) {
        close(); // Clean up previous initialization
    }
    
    width = w;
    height = h;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL Error: %s", SDL_GetError());
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Renderer could not be created! SDL Error: %s", SDL_GetError());
        return false;
    }
    
    // Create texture that will be updated with webcam data
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (texture == nullptr) {
        SDL_Log("Texture could not be created! SDL Error: %s", SDL_GetError());
        return false;
    }
    
    initialized = true;
    return true;
}

bool WebcamDisplay::displayFrame(const std::vector<Color>* pixelData) {
    if (!initialized || pixelData == nullptr) {
        return false;
    }
    
    if (pixelData->size() != static_cast<size_t>(width * height)) {
        SDL_Log("Error: Pixel data size (%zu) doesn't match window dimensions (%d x %d)", 
               pixelData->size(), width, height);
        return false;
    }
    
    // Lock texture for pixel manipulation
    void* pixels;
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        SDL_Log("Unable to lock texture! %s", SDL_GetError());
        return false;
    }
    
    // Copy pixel data from the vector to the texture
    Uint32* texturePixels = static_cast<Uint32*>(pixels);
    for (size_t i = 0; i < pixelData->size(); ++i) {
        texturePixels[i] = colorToUint32((*pixelData)[i]);
    }
    
    // Unlock texture
    SDL_UnlockTexture(texture);
    
    // Clear screen
    SDL_RenderClear(renderer);
    
    // Render texture to screen
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    
    // Update screen
    SDL_RenderPresent(renderer);
    
    return true;
}

void WebcamDisplay::close() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    if (initialized) {
        SDL_Quit();
        initialized = false;
    }
}