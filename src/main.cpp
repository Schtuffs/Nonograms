#include <iostream>
#include <print>

#include "raygui.h"
#include "raylib.h"
#include "rlgl.h"

#include "Grid.h"
#include "Utils.h"

constexpr Color HEADER_BACKGROUND = {200, 200, 200, 225};

void RenderGridTop(const Grid& grid, int size)
{
    int fontSize = 16;

    // Renders the headers - Top
    int width  = 0;
    int height = 152;
    int top    = -height;
    int left   = width;
    int mid    = size / 2;
    for (int i = 0; i < grid.Width(); ++i) {
        // Main square
        DrawRectangle(left + i * size, top, size, height, HEADER_BACKGROUND);
        DrawRectangleLines(left + i * size, top, size, height, BLACK);

        // Header data
        const auto& header = grid.Header(Grid::HEADER_TOP, i);
        for (u64 idx = 0; idx < header.size(); ++idx) {
            auto        info    = header[(header.size() - idx) - 1];
            std::string str     = std::to_string(info);
            Vector2     textPos = Utils::CenterText(str.data(), GetFontDefault(), fontSize, 1.,
                                                    {(float)mid, -(fontSize / 2.f)});
            DrawText(str.data(), textPos.x + size * i, textPos.y + textPos.y * idx, fontSize,
                     BLACK);
        }
    }
}

void RenderGridLine(const Grid& grid, int size, int i)
{
    const auto& header = grid.Header(Grid::HEADER_LEFT, i);
    const auto& cells  = grid.Row(i);

    int fontSize = 16;
    int width    = 152;
    int height   = 0;
    int top      = height;
    int left     = -width;
    int mid      = size / 2;

    // Main square
    DrawRectangle(left, top + i * size, width, size, HEADER_BACKGROUND);
    DrawRectangleLines(left, top + i * size, width, size, BLACK);

    // Header data
    int runningCount = -10;
    for (u64 idx = 0; idx < header.size(); ++idx) {
        auto        info    = header[(header.size() - idx) - 1];
        std::string str     = std::to_string(info);
        Vector2     textPos = Utils::CenterText(str.data(), GetFontDefault(), fontSize, 1.,
                                                {-(fontSize / 2.f), (float)mid});
        runningCount -= textPos.x - 5;
        DrawText(str.data(), textPos.x - runningCount, textPos.y + size * i, fontSize, BLACK);
    }

    // Cell data
    for (u64 idx = 0; idx < cells.size(); ++idx) {
        switch (cells[idx]) {
        case Grid::CELL_OPEN:
            DrawRectangle(idx * size, i * size, size, size, WHITE);
            break;
        case Grid::CELL_VALID:
            DrawRectangle(idx * size, i * size, size, size, BLACK);
            break;
        case Grid::CELL_INVALID:
            DrawRectangle(idx * size, i * size, size, size, WHITE);
            DrawLine(idx * size, i * size, idx * size + i * size, idx * size + i * size, BLACK);
            break;
        }

        DrawRectangleLines(idx * size, i * size, size, size, BLACK);
    }
}

void RenderGridLeft(const Grid& grid, int size)
{
    for (int i = 0; i < grid.Width(); ++i) {
        RenderGridLine(grid, size, i);
    }
}

void RenderGrid(const Grid& grid)
{
    Vector3 start  = Utils::GridPos(0.5f);
    int     startx = start.x;
    int     starty = start.y;
    int     size   = start.z / std::max(grid.Width(), grid.Height());

    rlPushMatrix();
    rlTranslatef(startx, starty, 0);

    RenderGridTop(grid, size);
    RenderGridLeft(grid, size);

    rlPopMatrix();
}

enum Menu {
    MENU_CLOSE,
    MENU_MAIN,
    MENU_CREATE,
};

void AnimateMenu(Grid& grid, double& time, int& frame)
{
    constexpr int    MAX_FRAMES              = 6;
    constexpr double FRAME_TIMES[MAX_FRAMES] = {0.25, 0.5, 0.75, 1., 1.25, 1.5};

    if (frame >= MAX_FRAMES) {
        return;
    }

    double cur = GetTime();
    if (cur >= FRAME_TIMES[frame]) {
        time = FRAME_TIMES[frame];
        switch (frame++) {
        case 0:
            grid = Grid(1, 1);
            break;
        case 1:
            grid = Grid(2, 2);
            break;
        case 2:
            grid = Grid(3, 3);
            break;
        case 3:
            grid = Grid(4, 4);
            break;
        case 4:
            grid = Grid(5, 5);
            break;
        case 5:
            grid = Grid(6, 6);
            break;
        case 6:
            break;
        }
    }
}

int ShowMenuMain(int menu, Grid& grid)
{
    int   width  = GetRenderWidth();
    int   height = GetRenderHeight();
    float wid    = width / 8;
    float hei    = height / 8;

    RenderGrid(grid);

    if (GuiButton({0, 0, wid, hei}, "Create puzzle")) {
        menu = MENU_CREATE;
    }
    if (GuiButton({wid, 0, wid, hei}, "Load puzzles")) {
        menu = MENU_CREATE;
    }
    if (GuiButton({wid, hei, wid, hei}, "Exit")) {
        menu = MENU_CLOSE;
    }

    return menu;
}

int ShowMenuCreate(int menu, Grid& grid)
{
    int   width  = GetRenderWidth();
    int   height = GetRenderHeight();
    float wid    = width / 8;
    float hei    = height / 8;

    RenderGrid(grid);

    if (GuiButton({wid, hei, wid, hei}, "Return")) {
        menu = MENU_MAIN;
    }

    return menu;
}

void UpdateUiElements()
{
    float value = std::max(std::min(GetRenderWidth(), GetRenderHeight()) / 50., 10.);
    GuiSetStyle(DEFAULT, TEXT_SIZE, value);
}

int main(void)
{
    SetConfigFlags(
        FLAG_WINDOW_RESIZABLE); // | FLAG_FULLSCREEN_MODE | FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(200, 200, "Nonograms");
    SetTargetFPS(60);
    SetWindowMinSize(600, 600);
    SetExitKey(KEY_NULL);
    UpdateUiElements();

    // Menu animation
    int    menu      = MENU_MAIN;
    int    animSize  = 1;
    double animTime  = GetTime();
    int    animFrame = 0;
    Grid   animGrid(animSize, animSize);

    Grid grid(15, 15);
    bool closeWindow = false;
    while (!WindowShouldClose() && !closeWindow) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            switch (menu) {
            case MENU_MAIN:
                closeWindow = true;
                break;
            case MENU_CREATE:
                menu = MENU_MAIN;
                break;
            }
        }

        if (IsWindowResized()) {
            UpdateUiElements();
        }

        BeginDrawing();
        ClearBackground({78, 159, 229, 255});

        switch (menu) {
        case MENU_MAIN:
            AnimateMenu(animGrid, animTime, animFrame);
            menu = ShowMenuMain(menu, animGrid);
            if (menu == MENU_CLOSE) {
                closeWindow = true;
            }
            break;
        case MENU_CREATE:
            menu = ShowMenuCreate(menu, grid);
            break;
        }

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
