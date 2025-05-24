#include "dependencies.h"

bool listcmp(std::pair<Pos, float> A, std::pair<Pos, float> B)
{
    return A.second > B.second;
}

Pos TrackedObjectsPosition(Pos lastPos, std::vector<Color>* pixels, std::vector<Color>* objAtt)
{
    Pos currentPos = lastPos;
    
    const int n = MATCHING_FRAME_SIZE;
    
    std::list<std::pair<Pos, float>> bestRegions;
    bestRegions.push_back({currentPos, 3*n*n});
    
    for(int y=n/2; y<WINDOW_HEIGHT-n/2; y+=n/2)
    {
        for(int x=n/2; x<WINDOW_WIDTH-n/2; x+=n/2)
        {
            std::vector<Color>square(n*n);

            int start = (y-n/2)*WINDOW_WIDTH+x-n/2;

            for(int iy=0; iy<n; iy++)
                for(int ix=0; ix<n; ix++)
                    SetPixel(&square, n, ix, iy, (*pixels)[start+iy*WINDOW_WIDTH+ix]);
            
            float dif = ComparePixels(objAtt, &square);
            
            if(dif < bestRegions.back().second)
            {
                if(bestRegions.size() > WINDOW_WIDTH*WINDOW_HEIGHT/n/n/10)
                    bestRegions.pop_back();

                bestRegions.push_back({{x, y}, dif});

                bestRegions.sort(listcmp);
            }
        }
    }

    float bestMatch = 3*n*n;
    for(std::pair<Pos, float> reg : bestRegions)
    {
        int x = reg.first.x, y = reg.first.y;

        std::vector<Color>square(n*n);

        int start = (y-n/2)*WINDOW_WIDTH+x-n/2;

        for(int iy=0; iy<n; iy++)
            for(int ix=0; ix<n; ix++)
                SetPixel(&square, n, ix, iy, (*pixels)[start+iy*WINDOW_WIDTH+ix]);

        float dif = ComparePixels(objAtt, &square);
            
        if(dif < bestMatch)
        {
            bestMatch = dif;
            currentPos = {x, y};
        }
    }

    return currentPos;
}
      
int main(int argv, char** args)
{   
    int* frame;
    
    WebcamCapture webcam(WINDOW_WIDTH, WINDOW_HEIGHT);

    std::string webcamName = webcam.getDeviceName();
    std::string windowTitle = "Tracking - Webcam Display (" + webcamName + ")";
    
    WebcamDisplay display;
    if (!display.initialize(windowTitle.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return 0;
    }

    std::cout<<"--- Keystrokes guide ---"<<std::endl;
    std::cout<<"F4 - toggle FPS cap (30 fps, enabled by default)"<<std::endl;
    std::cout<<"ESC - exit program"<<std::endl;

    bool running = true;
    SDL_Event e;

    const int n = MATCHING_FRAME_SIZE;

    int startDelay = START_DELAY;
    
    std::vector<Color> objAttribute(n*n);
    Pos objPos = {WINDOW_WIDTH/2-n/2, WINDOW_HEIGHT/2-n/2};
    
    bool capFPS = true;
    Timer fpsCapTimer;
    while(running)
    {   
        fpsCapTimer.start();
        
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_ESCAPE) running = false;
                //if(e.key.keysym.sym == SDLK_F3) showFPS ? showFPS = false : showFPS = true;
                if(e.key.keysym.sym == SDLK_F4)
                {
                    if(capFPS)
                    {
                        capFPS = false;
                        std::cout<<"FPS cap off."<<std::endl;
                    }
                    else
                    {
                        capFPS = true;
                        std::cout<<"FPS cap on."<<std::endl;
                    }
                }
            }
        }
        
        webcam.captureFrame(0);
        std::vector<Color>* pixels = webcam.getFrame();

        if(startDelay == 0)
        {
            int start = WINDOW_HEIGHT/2*WINDOW_WIDTH + WINDOW_WIDTH/2 - n/2 - n/2*WINDOW_WIDTH;

            for(int y=0; y<n; y++)
                for(int x=0; x<n; x++)
                    SetPixel(&objAttribute, n, x, y, (*pixels)[start+y*WINDOW_WIDTH+x]);

            startDelay--;
        }
        else if(startDelay == -1)
        {
            objPos = TrackedObjectsPosition(objPos, pixels, &objAttribute);

            int x = objPos.x, y = objPos.y;

            x = std::max(n/2+1, std::min(WINDOW_WIDTH-n/2-1, x));
            y = std::max(n/2+1, std::min(WINDOW_WIDTH-n/2-1, y));
            
            DrawSqFrame(pixels, n+2, x-n/2-1, y-n/2-1);
        }
        else
        {
            DrawSqFrame(pixels, n+2, WINDOW_WIDTH/2-n/2-1, WINDOW_HEIGHT/2-n/2-1);

            startDelay--;
        }

        // Display the frame
        if(!display.displayFrame(pixels)) std::cout<<"Current frame couldn't be displayed"<<std::endl;
        
        // Control frame rate
        if(fpsCapTimer.getTicks() < 1000/FPS_CAP && capFPS) SDL_Delay(1000/FPS_CAP-fpsCapTimer.getTicks());
    }
    
    display.close();
    webcam.~WebcamCapture();

    return 0;
}