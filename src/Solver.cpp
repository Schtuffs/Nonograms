#include "Solver.h"

#include <print>

namespace Solver {

void ReverseSolve(Grid& grid)
{
    // Vertical headers
    for (int x = 0; x < grid.Width(); x++) {
        auto& header = grid.Header(Grid::HEADER_VERT, x);
        header.clear();

        // Data
        int count = 0;
        for (int y = 0; y < grid.Height(); y++) {
            if (grid[x][y] == Grid::CELL_VALID) {
                count++;
            } else if (count) {
                header.push_back(count);
                count = 0;
            }
        }
        if (count || header.size() == 0) {
            header.push_back(count);
        }
    }

    // Horizontal headers
    for (int y = 0; y < grid.Height(); y++) {
        auto& header = grid.Header(Grid::HEADER_HORZ, y);
        header.clear();

        // Data
        int count = 0;
        for (int x = 0; x < grid.Width(); x++) {
            if (grid[x][y] == Grid::CELL_VALID) {
                count++;
            } else if (count) {
                header.push_back(count);
                count = 0;
            }
        }
        if (count || header.size() == 0) {
            header.push_back(count);
        }
    }
}

} // namespace Solver
