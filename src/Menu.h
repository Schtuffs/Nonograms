#pragma once

#include "Grid.h"

namespace Menu {

enum Menu {
    MENU_CLOSE,
    MENU_MAIN,
    MENU_CREATE,
};

// ----- Menus -----

void AnimateMenu(Grid& grid, double& time, int& frame);
int  Main(int menu, Grid& grid);
int  Create(int menu, Grid& grid);

// ----- Grid ----

void RenderGridTop(const Grid& grid, int size);
void RenderGridLine(const Grid& grid, int size, int i);
void RenderGridLeft(const Grid& grid, int size);
void RenderGrid(const Grid& grid);

} // namespace Menu
