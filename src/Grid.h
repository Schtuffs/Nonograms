#pragma once

#include <array>
#include <vector>

#include "Constants.h"
#include "ShortTypes.h"

class Grid {
public:
    enum HeaderType { HEADER_VERT, HEADER_HORZ, HEADER_MAX };
    enum Cell { CELL_OPEN, CELL_VALID, CELL_INVALID };

    // ----- Creation / Destruction -----

    Grid(int width, int height);
    ~Grid() = default;

    // ----- Read -----

    int                    Width() const;
    int                    Height() const;
    const std::vector<u8>& Header(HeaderType type, int line) const;

    std::vector<Cell>        Row(int row) const;
    std::vector<Cell>        Column(int column) const;
    Cell                     At(Point point) const;
    const std::vector<Cell>& operator[](int idx) const;

    bool Save(const char* filename) const;

    // ----- Update -----

    std::vector<u8>& Header(HeaderType type, int line);

    Cell&              At(Point point);
    std::vector<Cell>& operator[](int idx);

    void ResizeWidth(int newCount);
    void ResizeHeight(int newCount);

    bool Load(const char* filename);

private:
    // Array   - Left and Top
    // Vector1 - Each line (col or row)
    // Vector2 - Actual header data
    std::array<std::vector<std::vector<u8>>, HEADER_MAX> m_headers;
    std::vector<std::vector<Cell>>                       m_cells;
    int                                                  m_width, m_height;
};
