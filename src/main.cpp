#include "raylib.h"
#include <vector>
#include <iostream>

const int CELL = 60;

int binToInt(int a, int b, int c, int d)
{
    return a * 1 + b * 2 + c * 4 + d * 8;
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
    const int endX = 1 + screenWidth / CELL;
    const int endY = 1 + screenHeight / CELL;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Marching Squares");

    std::vector<std::vector<int>> plane(endX);

    for (int i{0}; i < endX; i++)
    {
        plane[i] = std::vector<int>(endY);
        for (int j{0}; j < endY; j++)
            plane[i][j] = GetRandomValue(0, 1);
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        for (int i{0}; i < endX; i++)
        {
            for (int j{0}; j < endY; j++)
            {
                //unsigned char col = 255 * plane[i][j];
                //DrawCircle(i * CELL, j * CELL, 2.0, Color{col, col, col, 255});
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
