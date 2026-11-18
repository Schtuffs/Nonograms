#include "Grid.h"

// ----- Creation / Destruction -----

Grid::Grid(int w, int h) : m_width(w), m_height(h)
{
    // Reserve cell spaces
    m_cells.reserve(w);
    for (int i = 0; i < w; i++) {
        m_cells.push_back(std::vector<Grid::Cell>(h));
    }

    // Vertical header data
    m_headers[HEADER_VERT].reserve(m_width);
    for (int i = 0; i < m_width; ++i) {
        m_headers[HEADER_VERT].push_back(std::vector<u8>(1));
    }

    // Horizontal header data
    m_headers[HEADER_HORZ].reserve(m_height);
    for (int i = 0; i < m_height; ++i) {
        m_headers[HEADER_HORZ].push_back(std::vector<u8>(1));
    }
}

void Grid::ResizeWidth(int s)
{
    if (s < 1) {
        s = 1;
    }

    m_cells.resize(s);
    m_headers[Grid::HEADER_VERT].resize(s);
    // if (s > m_width) {
    //     m_cells[s - 1] = std::vector<Grid::Cell>(m_height);
    // }
    m_width = s;
}

void Grid::ResizeHeight(int s)
{
    if (s < 1) {
        s = 1;
    }
    
    for (u64 i = 0; i < m_cells.size(); i++) {
        m_cells[i].resize(s);
        m_headers[Grid::HEADER_HORZ][i].resize(s);
    }
    m_height = s;
}

// ----- Read -----

int Grid::Width() const { return m_width; }

int Grid::Height() const { return m_height; }

const std::vector<u8>& Grid::Header(Grid::HeaderType type, int line) const
{
    return m_headers[type][line];
}

std::vector<Grid::Cell> Grid::Row(int row) const
{
    std::vector<Grid::Cell> cells;

    cells.reserve(m_cells.size());
    for (u64 i = 0; i < m_cells.size(); i++) {
        cells.push_back(m_cells[i][row]);
    }

    return cells;
}

std::vector<Grid::Cell> Grid::Column(int column) const { return m_cells[column]; }

Grid::Cell Grid::At(Point point) const { return m_cells[point.x][point.y]; }

const std::vector<Grid::Cell>& Grid::operator[](int idx) const { return m_cells[idx]; }

// ----- Update -----

std::vector<u8>& Grid::Header(Grid::HeaderType type, int line) { return m_headers[type][line]; }

Grid::Cell& Grid::At(Point point) { return m_cells[point.x][point.y]; }

std::vector<Grid::Cell>& Grid::operator[](int idx) { return m_cells[idx]; }
