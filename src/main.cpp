#include "raylib.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>

const int CELL = 5;

int binToInt(int a, int b, int c, int d)
{
    return a * 1 + b * 2 + c * 4 + d * 8;
}

Vector2 randomGrad(int ix, int iy) {
    float random = 2920.f * sin(ix * 21942.f + iy * 171324.f + 8912.f) * cos(ix * 23157.f * iy * 217832.f + 9758.f) * rand();
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

void drawContour(const std::vector<std::vector<int>> &plane, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4)
{
    Vector2 rlP1 = Vector2{p1.x * CELL + CELL/2, p1.y * CELL};
    Vector2 rlP2 = Vector2{p2.x * CELL, p2.y * CELL + CELL/2};
    Vector2 rlP3 = Vector2{p3.x * CELL, p3.y * CELL - CELL/2};
    Vector2 rlP4 = Vector2{p4.x * CELL - CELL/2, p4.y * CELL};

    switch (binToInt(plane[p1.x][p1.y], plane[p2.x][p2.y], plane[p3.x][p3.y], plane[p4.x][p4.y]))
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

    double dtime = 0.0;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Marching Squares");

    std::vector<std::vector<int>> plane(endX);

    for (int i{0}; i < endX; i++)
    {
        plane[i] = std::vector<int>(endY);
        for (int j{0}; j < endY; j++)
        {
            if (i != endX-1 && j != endY-1)
            {
                float rngx = rand() / RAND_MAX;
                float rngy = rand() / RAND_MAX;
                plane[i][j] = ceil(perlinNoise(i + rngx, j + rngy, 6, 1.0 / 4.0, 2.0));
            }
        }
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        /*
        dtime += GetFrameTime();
        if (dtime >= 0.5)
        {
            dtime = 0.0;
            for (int i{0}; i < endX; i++)
            {
                plane[i] = std::vector<int>(endY);
                for (int j{0}; j < endY; j++)
                {
                    if (i != endX-1 && j != endY-1)
                    {
                        float rngx = rand() / RAND_MAX;
                        float rngy = rand() / RAND_MAX;
                        plane[i][j] = perlinNoise(i + rngx, j + rngy, 6, 1.0 / 8.0, 2.0) < 0.5 ? 0 : 1;
                    }
                }
            }
        }
        */

        ClearBackground(RAYWHITE);
        for (int i{0}; i < endX; i++)
        {
            for (int j{0}; j < endY; j++)
            {
                if (i != endX-1 && j != endY-1)
                {
                    Vector2 p1 = Vector2{(float)i, (float)j};
                    Vector2 p2 = Vector2{(float)i + 1, (float)j};
                    Vector2 p3 = Vector2{(float)i, (float)j + 1};
                    Vector2 p4 = Vector2{(float)i + 1, (float)j + 1};
                    drawContour(plane, p1, p2, p3, p4);
                }
            }
        }   

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
