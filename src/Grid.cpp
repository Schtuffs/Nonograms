#include "Grid.h"

#include <cstdio>
#include <iostream>
#include <print>

#define COMMA ""

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

bool Grid::Save(const char* filename) const
{
    if (!filename) {
        return false;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        return false;
    }

    std::println(file, "{},{}", Width(), Height());

    for (int i = 0; i < Width(); i++) {
        const auto& header = Header(Grid::HEADER_VERT, i);
        std::print(file, "{}", header.size());
        for (const auto& item : header) {
            std::print(file, ",{}", item);
        }
        std::println(file);
    }

    for (int i = 0; i < Height(); i++) {
        const auto& header = Header(Grid::HEADER_HORZ, i);
        std::print(file, "{}", header.size());
        for (const auto& item : header) {
            std::print(file, ",{}", item);
        }
        std::println(file);
    }

    fclose(file);
    return true;
}

// ----- Update -----

std::vector<u8>& Grid::Header(Grid::HeaderType type, int line) { return m_headers[type][line]; }

Grid::Cell& Grid::At(Point point) { return m_cells[point.x][point.y]; }

std::vector<Grid::Cell>& Grid::operator[](int idx) { return m_cells[idx]; }

void Grid::ResizeWidth(int s)
{
    if (s < 1) {
        s = 1;
    }

    m_cells.resize(s);
    m_headers[Grid::HEADER_VERT].resize(s);
    m_width = s;

    for (auto& line : m_cells) {
        if (line.size() == 0) {
            line.resize(m_height);
        }
    }

    for (auto& header : m_headers[Grid::HEADER_VERT]) {
        if (header.size() == 0) {
            header.resize(1);
        }
    }
}

void Grid::ResizeHeight(int s)
{
    if (s < 1) {
        s = 1;
    }

    m_headers[Grid::HEADER_HORZ].resize(s);
    for (u64 i = 0; i < m_cells.size(); i++) {
        m_cells[i].resize(s);
    }
    m_height = s;

    for (auto& header : m_headers[Grid::HEADER_HORZ]) {
        if (header.size() == 0) {
            header.resize(1);
        }
    }
}

static bool LoadHeader(FILE* file, int edgeSize, std::vector<std::vector<u8>>& header)
{
    for (int i = 0; i < edgeSize; i++) {
        int count;
        if (fscanf(file, "%d", &count) != 1) {
            fclose(file);
            return false;
        }

        std::vector<u8> data(count);
        for (int j = 0; j < count; j++) {
            u8 val;
            if (fscanf(file, ",%hhu", &val) != 1) {
                fclose(file);
                return false;
            }
            data.push_back(val);
        }

        header[i] = data;
    }

    return true;
}

bool Grid::Load(const char* filename)
{
    if (!filename) {
        return false;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        return false;
    }

    int wid, hei;
    if (fscanf(file, "%d,%d", &wid, &hei) != 2) {
        fclose(file);
        return false;
    }

    ResizeWidth(wid);
    ResizeHeight(hei);

    if (!LoadHeader(file, m_width, m_headers[Grid::HEADER_VERT])) {
        return false;
    }
    if (!LoadHeader(file, m_height, m_headers[Grid::HEADER_HORZ])) {
        return false;
    }

    fclose(file);
    std::println("YIPPEE");
    return true;
}
