#include "raylib.h"
#include <ctime>
#include <vector>

const int CELL = 10;

int binToInt(int a, int b, int c, int d)
{
    return a * 1 + b * 2 + c * 4 + d * 8;
}

void drawContour(const std::vector<std::vector<float>> &plane, int px, int py)
{

    float val1 = plane[px][py];
    float val2 = plane[px + 1][py];
    float val3 = plane[px + 1][py + 1];
    float val4 = plane[px][py + 1];

    float cx = px * CELL;
    float cy = py * CELL;

    Vector2 rlP1 = Vector2{cx + CELL/2, cy};
    Vector2 rlP2 = Vector2{cx + CELL, cy + CELL/2};
    Vector2 rlP3 = Vector2{cx, cy + CELL/2};
    Vector2 rlP4 = Vector2{cx + CELL/2, cy + CELL};

    switch (binToInt(val1, val2, val4, val3))
    {
    case 1:
    case 14:
        DrawLineV(rlP1, rlP3, RAYWHITE);
        break;
    case 2:
    case 13:
        DrawLineV(rlP1, rlP2, RAYWHITE);
        break;
    case 3:
    case 12:
        DrawLineV(rlP3, rlP2, RAYWHITE);
        break;
    case 4:
    case 11:
        DrawLineV(rlP3, rlP4, RAYWHITE);
        break;
    case 5:
    case 10:
        DrawLineV(rlP1, rlP4, RAYWHITE);
        break;
    case 6:
        DrawLineV(rlP1, rlP3, RAYWHITE);
        DrawLineV(rlP2, rlP4, RAYWHITE);
        break;
    case 7:
    case 8:
        DrawLineV(rlP2, rlP4, RAYWHITE);
        break;
    case 9:
        DrawLineV(rlP1, rlP2, RAYWHITE);
        DrawLineV(rlP3, rlP4, RAYWHITE);
        break;
    }
}

int getNeighbours(int x, int y, int endX, int endY, const std::vector<std::vector<float>> &plane)
{
    int wallCount = 0;
    for (int nx = x - 1; nx <= x + 1; nx++) {
	for (int ny = y - 1; ny <= y + 1; ny++) {
	    if (nx >= 0 && nx < endX && ny >= 0 && ny < endY) {
		if (nx != x || ny != y) {
		    wallCount += plane[nx][ny];
		}
	    }
	    else {
		wallCount++;
	    }
	}
    }
    return wallCount;
}

int main()
{
    const int screenWidth = 900;
    const int screenHeight = 600;
    const int endX = 2 + screenWidth / CELL;
    const int endY = 2 + screenHeight / CELL;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Marching Squares");

    std::vector<std::vector<float>> plane(endX);
    int randPerc = 50;
    RenderTexture2D drawTexture = LoadRenderTexture(screenWidth, screenHeight);

    for (int i{0}; i < endX; i++)
    {
        plane[i] = std::vector<float>(endY);
        for (int j{0}; j < endY; j++)
        {
            if (i != endX - 2 && j != endY - 2 && i != 0 && j != 0)
                plane[i][j] = GetRandomValue(0, 100) < randPerc ? 1 : 0;
            else
                plane[i][j] = 1;
        }
    }

    for (int _k{0}; _k < 8; _k++)
    {
        for (int i{0}; i < endX; i++)
        {
            for (int j{0}; j < endY; j++)
            {
                int neighbours = getNeighbours(i, j, endX, endY, plane);
                if (neighbours > GetRandomValue(4, 5))
                    plane[i][j] = 1;
                else if (neighbours < 4)
                    plane[i][j] = 0;
            }
        }
    }
    BeginTextureMode(drawTexture);
        ClearBackground(BLACK);
        for (int i{0}; i < endX; i++)
        {
            for (int j{0}; j < endY; j++)
            {
                if (i != endX - 1 && j != endY - 1)
                    drawContour(plane, i, j);
                    if (plane[i][j] == 1)
                        DrawRectangle(i * CELL, j * CELL, 2.0, 2.0, RAYWHITE);
            }
        }
    EndTextureMode();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(drawTexture.texture, 0, 0, WHITE);
        EndDrawing();
    }
    UnloadRenderTexture(drawTexture);
    CloseWindow();
    return 0;
}
