#include "Menu.h"

#include <print>
#include <string>

#include "raygui.h"
#include "raylib.h"
#include "rlgl.h"

#include "Solver.h"
#include "Utils.h"

namespace Menu {

// ----- Globals -----

constexpr Color HEADER_BACKGROUND = {200, 200, 200, 225};

constexpr int BUTTON1 = 0b0000'0001;
constexpr int BUTTON2 = 0b0000'0010;
constexpr int BUTTON3 = 0b0000'0100;
constexpr int BUTTON4 = 0b0000'1000;

constexpr float GRID_SIZE = 0.5;

constexpr int FONT_SIZE = 20;
constexpr int BOX_SIZE  = 150;

typedef struct Globals {
    struct {
        Grid::HeaderType headerType  = Grid::HEADER_HORZ;
        int              headerIndex = 0;
    } Create;
} Globals;

Globals STATE;

// ------ Helpers -----

bool IsBetween(float value, float min, float max) { return (min <= value && value <= max); }

int GuiButtons(const char* b1, const char* b2, const char* b3, const char* b4)
{
    int   width  = GetRenderWidth();
    int   height = GetRenderHeight();
    float wid    = width / 8;
    float hei    = height / 8;

    int res = 0;

    if (b1) {
        if (GuiButton({0, 0, wid, hei}, b1)) {
            res |= BUTTON1;
        }
    }
    if (b2) {
        if (GuiButton({wid, 0, wid, hei}, b2)) {
            res |= BUTTON2;
        }
    }
    if (b3) {
        if (GuiButton({0, hei, wid, hei}, b3)) {
            res |= BUTTON3;
        }
    }
    if (b4) {
        if (GuiButton({wid, hei, wid, hei}, b4)) {
            res |= BUTTON4;
        }
    }

    return res;
}

bool ButtonClicked(int res, int button) { return (res & button); }

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

void ToggleCell(Grid& grid, Vector2 pos, Grid::Cell value)
{
    int x = pos.x;
    int y = pos.y;

    Grid::Cell& cell = grid[x][y];
    switch (cell) {
    case Grid::CELL_OPEN:
        cell = value;
        break;
    case Grid::CELL_VALID:
        if (cell == value) {
            cell = Grid::CELL_OPEN;
        } else {
            cell = value;
        }
        break;
    case Grid::CELL_INVALID:
        if (cell == value) {
            cell = Grid::CELL_OPEN;
        } else {
            cell = value;
        }
        break;
    }
}

// ----- Menus -----

int Main(int menu, Grid& grid)
{
    int res = GuiButtons("Create puzzle", "Load puzzle", NULL, "Exit");

    if (ButtonClicked(res, BUTTON1)) {
        menu = Menu::MENU_CREATE;
    }
    if (ButtonClicked(res, BUTTON2)) {
        std::println("TODO: {}: BUTTON2 Load puzzle", MACRO_FUNCTION);
        menu = Menu::MENU_MAIN;
    }
    if (ButtonClicked(res, BUTTON4)) {
        menu = Menu::MENU_CLOSE;
    }

    RenderGrid(grid);

    return menu;
}

int Create(int menu, Grid& grid)
{
    RenderGrid(grid);

    int  res        = GuiButtons(NULL, NULL, NULL, "Return");
    bool mouseLeft  = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool mouseRight = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    float wid = GetScreenWidth() / 8;
    float hei = GetScreenHeight() / 8;

    std::string str = "Grid Width: " + std::to_string(grid.Width());
    GuiButton({wid * 6, hei * 6, wid, hei}, str.c_str());

    str = "Grid Height: " + std::to_string(grid.Height());
    GuiButton({wid * 6, hei * 7, wid, hei}, str.c_str());

    // Detect button presses.
    if (ButtonClicked(res, BUTTON4)) {
        menu                     = MENU_MAIN;
        STATE.Create.headerType  = Grid::HEADER_VERT;
        STATE.Create.headerIndex = 0;
    }
    // Grid width--
    else if (GuiButton({wid * 5, hei * 6, wid, hei}, "-")) {
        grid.ResizeWidth(grid.Width() - 1);
        Solver::ReverseSolve(grid);
    }
    // Grid width++
    else if (GuiButton({wid * 7, hei * 6, wid, hei}, "+")) {
        grid.ResizeWidth(grid.Width() + 1);
        Solver::ReverseSolve(grid);
    }
    // Grid height--
    else if (GuiButton({wid * 5, hei * 7, wid, hei}, "-")) {
        grid.ResizeHeight(grid.Height() - 1);
        Solver::ReverseSolve(grid);
    }
    // Grid height++
    else if (GuiButton({wid * 7, hei * 7, wid, hei}, "+")) {
        grid.ResizeHeight(grid.Height() + 1);
        Solver::ReverseSolve(grid);
    }
    // Otherwise can check for grid clicks.
    else if (mouseLeft || mouseRight) {
        // Determine positions
        Vector3 gp   = Utils::GridPos(GRID_SIZE);
        Vector2 mp   = GetMousePosition();
        int     size = gp.z / std::max(grid.Width(), grid.Height());

        // Is in grid, find square
        if (IsBetween(mp.x, gp.x - BOX_SIZE, gp.x + gp.z) &&
            IsBetween(mp.y, gp.y - BOX_SIZE, gp.y + gp.z)) {
            Vector2 pos = {(mp.x - gp.x) / size, (mp.y - gp.y) / size};

            // Check indexing
            if (IsBetween(pos.x, 0, grid.Width()) && IsBetween(pos.y, 0, grid.Height())) {
                Grid::Cell cellType = (mouseLeft ? Grid::CELL_VALID : Grid::CELL_INVALID);
                ToggleCell(grid, pos, cellType);
                Solver::ReverseSolve(grid);
            } else if (pos.x < 0 && pos.y > 0) {
                STATE.Create.headerType  = Grid::HEADER_HORZ;
                STATE.Create.headerIndex = pos.y;
            } else if (pos.y < 0 && pos.x > 0) {
                STATE.Create.headerType  = Grid::HEADER_VERT;
                STATE.Create.headerIndex = pos.x;
            }
        }
    }

    return menu;
}

// ----- Grid -----

void RenderGridTop(const Grid& grid, int size)
{
    // Renders the headers - Top
    int height = BOX_SIZE;
    int top    = -height;
    int mid    = size / 2;
    for (int i = 0; i < grid.Width(); ++i) {
        // Main square
        DrawRectangle(i * size, top, size, height, HEADER_BACKGROUND);
        DrawRectangleLines(i * size, top, size, height, BLACK);

        // Header data
        const auto& header = grid.Header(Grid::HEADER_VERT, i);
        for (u64 idx = 0; idx < header.size(); ++idx) {
            auto        info    = header[(header.size() - idx) - 1];
            std::string str     = std::to_string(info);
            Vector2     textPos = Utils::CenterText(str.data(), GetFontDefault(), FONT_SIZE, 1.,
                                                    {(float)mid, -(FONT_SIZE / 2.f)});
            DrawText(str.data(), textPos.x + size * i, textPos.y + textPos.y * idx, FONT_SIZE,
                     BLACK);
        }
    }
}

void RenderGridLineHeader(const std::vector<u8>& header, int size, int y)
{
    int textWidth = MeasureText("9", FONT_SIZE) * 1.1;
    int width     = BOX_SIZE;
    int left      = -width;
    int mid       = size / 2;

    // Main square
    DrawRectangle(left, y, width, size, HEADER_BACKGROUND);
    DrawRectangleLines(left, y, width, size, BLACK);

    // Render header text
    float charCount = 0;
    for (u64 idx = 0; idx < header.size(); ++idx) {
        auto info = header[(header.size() - idx) - 1];

        if (info >= 10) {
            int ones = info % 10;
            info -= ones;
            info /= 10;
            info = info % 10;

            char str[2]     = {0};
            str[0]          = ones + '0';
            Vector2 textPos = Utils::CenterText(str, GetFontDefault(), FONT_SIZE, 1.,
                                                {-(FONT_SIZE / 2.f), (float)mid});
            DrawText(str, textPos.x - (textWidth * (charCount++)), textPos.y + 1 + y, FONT_SIZE,
                     BLACK);

            str[0]  = info + '0';
            textPos = Utils::CenterText(str, GetFontDefault(), FONT_SIZE, 1.,
                                        {-(FONT_SIZE / 2.f), (float)mid});
            DrawText(str, textPos.x - (textWidth * (charCount++)), textPos.y + 1 + y, FONT_SIZE,
                     BLACK);
        } else {
            char str[2]     = {0};
            str[0]          = info + '0';
            Vector2 textPos = Utils::CenterText(str, GetFontDefault(), FONT_SIZE, 1.,
                                                {-(FONT_SIZE / 2.f), (float)mid});
            DrawText(str, textPos.x - (textWidth * (charCount++)), textPos.y + 1 + y, FONT_SIZE,
                     BLACK);
        }

        // Account for space
        charCount += 0.5;
    }
}

void RenderGridLineCells(const std::vector<Grid::Cell>& cells, int size, int y)
{
    // Cell data
    for (u64 idx = 0; idx < cells.size(); ++idx) {
        switch (cells[idx]) {
        case Grid::CELL_OPEN:
            DrawRectangle(idx * size, y, size, size, WHITE);
            break;
        case Grid::CELL_VALID:
            DrawRectangle(idx * size, y, size, size, BLACK);
            break;
        case Grid::CELL_INVALID:
            DrawRectangle(idx * size, y, size, size, WHITE);
            DrawLine(idx * size, y, idx * size + y, idx * size + y, BLACK);
            break;
        }

        DrawRectangleLines(idx * size, y, size, size, BLACK);
    }
}

void RenderGridLine(const Grid& grid, int size, int i, int y)
{
    const auto& header = grid.Header(Grid::HEADER_HORZ, i);
    auto        cells  = grid.Row(i);

    RenderGridLineHeader(header, size, y);
    RenderGridLineCells(cells, size, y);
}

void RenderGridLeft(const Grid& grid, int size)
{
    for (int i = 0; i < grid.Height(); ++i) {
        RenderGridLine(grid, size, i, i * size);
    }
}

void RenderGrid(const Grid& grid)
{
    Vector3 start  = Utils::GridPos(GRID_SIZE);
    int     startx = start.x;
    int     starty = start.y;
    int     size   = start.z / std::max(grid.Width(), grid.Height());

    rlPushMatrix();
    rlTranslatef(startx, starty, 0);

    RenderGridTop(grid, size);
    RenderGridLeft(grid, size);

    RenderGridLineHeader(grid.Header(STATE.Create.headerType, STATE.Create.headerIndex), size,
                         GetRenderHeight() * 0.583333);
    if (STATE.Create.headerType == Grid::HEADER_HORZ) {
        RenderGridLineCells(grid.Row(STATE.Create.headerIndex), size,
                            GetRenderHeight() * 0.8333333);
    } else {
        RenderGridLineCells(grid.Column(STATE.Create.headerIndex), size,
                            GetRenderHeight() * 0.8333333);
    }

    rlPopMatrix();
}

} // namespace Menu
