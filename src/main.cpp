#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

const int CELL = 30;

int binToInt(int a, int b, int c, int d)
{
    return a * 1 + b * 2 + c * 4 + d * 8;
}


Vector2 randomGrad(int ix, int iy)
{
    float random =
        2920.f * sin(ix * 21942.f + iy * 171324.f + 8912.f) * cos(ix * 23157.f * iy * 217832.f + 9758.f) * rand();
    return Vector2{(float)cos(random), (float)sin(random)};
}

float dotProd(float x1, float y1, Vector2 grad)
{
    return x1 * grad.x + y1 * grad.y;
}

float interp(float a0, float a1, float w)
{
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

float lerp(float v0, float v1, float t)
{
    return (1.0 - t) * v0 + v1 * t;
}

float noise2D(float x, float y)
{
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    float s = dotProd(x - x0, y - y0, randomGrad(x0, y0));
    float t = dotProd(x - x1, y - y0, randomGrad(x1, y0));
    float u = dotProd(x - x0, y - y1, randomGrad(x0, y1));
    float v = dotProd(x - x1, y - y1, randomGrad(x1, y1));

    float sx = 3.0 * powf(x - x0, 2.0) - 2.0 * powf(x - x0, 3.0);
    float sy = 3.0 * powf(y - y0, 2.0) - 2.0 * powf(y - y0, 3.0);
    float a = interp(s, t, sx);
    float b = interp(u, v, sx);

    float value = interp(b, a, sy);
    return value;
}

float perlinNoise(float x, float y, int octaves, float pers, float lum)
{
    float amp = 1.0;
    float total = 0.0;
    float freq = 1.0;
    float max = 0.0;

    for (int i{0}; i < octaves; i++)
    {
        freq = powf(lum, i);
        amp = powf(pers, i);
        total += noise2D(x * freq, y * freq) * amp;
        max += amp;
    }
    return total / max;
}

void drawContour(const std::vector<std::vector<float>> &plane, int px, int py)
{

    float val1 = plane[px][py] + 1.0;
    float val2 = plane[px + 1][py] + 1.0;
    float val3 = plane[px + 1][py + 1] + 1.0;
    float val4 = plane[px][py + 1] + 1.0;

    float cx = px * CELL;
    float cy = py * CELL;

    Vector2 rlP1 = Vector2{lerp(cx, cx + CELL, (1.0 - val1) / (val2 - val1)), cy};
    Vector2 rlP2 = Vector2{cx + CELL, lerp(cy, cy + CELL, (1.0 - val2) / (val3 - val2))};
    Vector2 rlP3 = Vector2{cx, lerp(cy, cy + CELL, (1.0 - val1) / (val4 - val1))};
    Vector2 rlP4 = Vector2{lerp(cx, cx + CELL, (1.0 - val4) / (val3 - val4)), cy + CELL};

    switch (binToInt(ceil(val1 - 1.0), ceil(val2 - 1.0), ceil(val4 - 1.0), ceil(val3 - 1.0)))
    {
    case 1:
    case 14:
        DrawLineV(rlP1, rlP3, BLACK);
        break;
    case 2:
    case 13:
        DrawLineV(rlP1, rlP2, BLACK);
        break;
    case 3:
    case 12:
        DrawLineV(rlP3, rlP2, BLACK);
        break;
    case 4:
    case 11:
        DrawLineV(rlP3, rlP4, BLACK);
        break;
    case 5:
    case 10:
        DrawLineV(rlP1, rlP4, BLACK);
        break;
    case 6:
        DrawLineV(rlP1, rlP3, BLACK);
        DrawLineV(rlP2, rlP4, BLACK);
        break;
    case 7:
    case 8:
        DrawLineV(rlP2, rlP4, BLACK);
        break;
    case 9:
        DrawLineV(rlP1, rlP2, BLACK);
        DrawLineV(rlP3, rlP4, BLACK);
        break;
    }
}

int main()
{
    const int screenWidth = 900;
    const int screenHeight = 600;
    const int endX = 2 + screenWidth / CELL;
    const int endY = 2 + screenHeight / CELL;

    srand(std::time(0));

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Marching Squares");

    std::vector<std::vector<float>> plane(endX);

    float xoff = 0.0;
    float yoff = 0.0;
    for (int i{0}; i < endX; i++)
    {
        xoff += 0.1;
        plane[i] = std::vector<float>(endY);
        for (int j{0}; j < endY; j++)
        {
            if (i != endX - 1 && j != endY - 1)
                plane[i][j] = perlinNoise(xoff, yoff, 6, 1.0 / 4.0, 2.0);
            yoff += 0.1;
        }
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for (int i{0}; i < endX; i++)
        {
            for (int j{0}; j < endY; j++)
            {
                if (i != endX - 1 && j != endY - 1)
                    drawContour(plane, i, j);
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
