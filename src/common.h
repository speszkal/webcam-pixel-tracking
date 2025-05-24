#pragma once

struct Pos
{
  int x;
  int y;
};

struct Color
{
    float r, g, b;

    Color();
    Color(float red, float green, float blue);
    ~Color();
};

Uint32 colorToUint32(const Color& color);

void SetPixel(std::vector<Color>* pixels, int width, int x, int y, Color rgb);

void DrawSqFrame(std::vector<Color>* pixels, int a, int x, int y); //(x, y) is the top left corner of the frame, a is the lenght of the outer face of the frame

float ComparePixels(std::vector<Color>* a, std::vector<Color>* b);