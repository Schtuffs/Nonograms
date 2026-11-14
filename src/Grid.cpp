#include "Grid.h"

// ----- Creation / Destruction -----

Grid::Grid(int w, int h) : m_width(w), m_height(h)
{
    //
    m_cells.reserve(w);
    for (int i = 0; i < w; i++) {
        m_cells.push_back(std::vector<Grid::Cell>(h));
    }

    for (int i = 0; i < m_width; ++i) {
        m_headers[HEADER_TOP].push_back(std::vector<u8>(1));
        m_headers[HEADER_TOP][i][0] = i + 1;
        for (int j = 0; j < i; ++j) {
            m_headers[HEADER_TOP][i].push_back(i * (j + 1));
        }
    }

    for (int i = 0; i < m_height; ++i) {
        m_headers[HEADER_LEFT].push_back(std::vector<u8>(1));
        m_headers[HEADER_LEFT][i][0] = i + 1;
        for (int j = 0; j < i; ++j) {
            m_headers[HEADER_LEFT][i].push_back(i * (j + 1));
        }
    }
}

// ----- Read -----

int Grid::Width() const noexcept { return m_width; }

int Grid::Height() const noexcept { return m_height; }

const std::vector<u8>& Grid::Header(Grid::HeaderType type, int line) const noexcept
{
    return m_headers[type][line];
}

std::vector<Grid::Cell>& Grid::Row(int row) { return m_cells[row]; }

const std::vector<Grid::Cell>& Grid::Row(int row) const noexcept { return m_cells[row]; }

std::vector<Grid::Cell>& Grid::Column(int column) { return Row(column); }

const std::vector<Grid::Cell>& Grid::Column(int column) const noexcept { return Row(column); }

// ----- Update -----

std::vector<u8>& Grid::Header(Grid::HeaderType type, int line) { return m_headers[type][line]; }
