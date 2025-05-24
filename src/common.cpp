#include "../dependencies.h"

Color::Color()
{}

Color::Color(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

Color::~Color()
{}

Uint32 colorToUint32(const Color& color) {
    // Convert floating-point RGB (0.0-1.0) to 8-bit per channel and pack into RGBA
    Uint8 r = static_cast<Uint8>(color.r * 255.0f);
    Uint8 g = static_cast<Uint8>(color.g * 255.0f);
    Uint8 b = static_cast<Uint8>(color.b * 255.0f);
    Uint8 a = 255;

    return (a << 24) | (b << 16) | (g << 8) | r; // RGBA format with full alpha
}

void SetPixel(std::vector<Color>* pixels, int width, int x, int y, Color rgb)
{
    (*pixels)[y*width+x] = rgb;
}

void DrawSqFrame(std::vector<Color>* pixels, int a, int x, int y) //(x, y) is the top left corner of the frame, a is the lenght of the outer face of the frame
{
    Color red = {0, 1, 0};

    int dx[] = {1, 0, -1, 0}; // Right, Down, Left, Up
    int dy[] = {0, 1, 0, -1};

    for (int dir = 0; dir < 4; dir++) {
        for (int i = 0; i < a-1; i++) {
            SetPixel(pixels, WINDOW_WIDTH, x, y, red);
            x += dx[dir];
            y += dy[dir];
        }
    }
}

float ComparePixels(std::vector<Color>* a, std::vector<Color>* b)
{
    const int n = MATCHING_FRAME_SIZE;
    
    float dif = 0;

    for(int y=0; y<n; y++)
    {
        for(int x=0; x<n; x++)
        {
            Color pA = (*a)[y*n+x];
            Color pB = (*b)[y*n+x];
            
            dif += (pA.r-pB.r)*(pA.r-pB.r) + (pA.g-pB.g)*(pA.g-pB.g) + (pA.b-pB.b)*(pA.b-pB.b);
        }
    }

    return dif;
}