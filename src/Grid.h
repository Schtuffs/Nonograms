#pragma once

#include <array>
#include <vector>

#include "ShortTypes.h"

class Grid {
public:
    enum HeaderType { HEADER_TOP, HEADER_LEFT, HEADER_MAX };
    enum Cell { CELL_OPEN, CELL_VALID, CELL_INVALID };

    // ----- Creation / Destruction -----

    Grid(int width, int height);
    ~Grid() = default;

    // ----- Read -----

    int                    Width() const noexcept;
    int                    Height() const noexcept;
    const std::vector<u8>& Header(HeaderType type, int line) const noexcept;

    std::vector<Cell>&       Row(int row);
    const std::vector<Cell>& Row(int row) const noexcept;

    std::vector<Cell>&       Column(int column);
    const std::vector<Cell>& Column(int column) const noexcept;

    // ----- Update -----

    std::vector<u8>& Header(HeaderType type, int line);

private:
    // Array   - Left and Top
    // Vector1 - Each line (col or row)
    // Vector2 - Actual header data
    std::array<std::vector<std::vector<u8>>, HEADER_MAX> m_headers;
    std::vector<std::vector<Cell>>                       m_cells;
    int                                                  m_width, m_height;
};
