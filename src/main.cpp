#include <iostream>
#include <print>

#include "raygui.h"
#include "raylib.h"

#include "Grid.h"
#include "Menu.h"

void UpdateUiElements()
{
    float value = std::max(std::min(GetRenderWidth(), GetRenderHeight()) / 50., 10.);
    GuiSetStyle(DEFAULT, TEXT_SIZE, value);
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(200, 200, "Nonograms");
    // SetTargetFPS(60);
    SetWindowMinSize(600, 600);
    SetExitKey(KEY_NULL);
    UpdateUiElements();

    // Menu animation
    int    menu      = Menu::MENU_MAIN;
    int    animSize  = 1;
    double animTime  = GetTime();
    int    animFrame = 0;
    Grid   animGrid(animSize, animSize);

    Grid grid(3, 3);
    bool closeWindow = false;
    int  s           = 1;
    while (!WindowShouldClose() && !closeWindow) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleBorderlessWindowed();
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            switch (menu) {
            case Menu::MENU_MAIN:
                closeWindow = true;
                break;
            case Menu::MENU_CREATE:
                menu = Menu::MENU_MAIN;
                break;
            }
        }

        if (IsKeyPressed(KEY_EQUAL)) {
            s++;
            animGrid = Grid(s, s);
        }
        if (IsKeyPressed(KEY_MINUS)) {
            s--;
            if (s < 1) {
                s = 1;
            }
            animGrid = Grid(s, s);
        }

        if (IsWindowResized()) {
            UpdateUiElements();
        }

        BeginDrawing();
        ClearBackground({78, 159, 229, 255});

        switch (menu) {
        case Menu::MENU_MAIN:
            Menu::AnimateMenu(animGrid, animTime, animFrame);
            menu = Menu::Main(menu, animGrid);
            if (menu == Menu::MENU_CLOSE) {
                closeWindow = true;
            }
            break;
        case Menu::MENU_CREATE:
            menu = Menu::Create(menu, grid);
            break;
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
