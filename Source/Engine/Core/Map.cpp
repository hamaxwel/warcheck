#include "Map.h"
#include <fstream>
#include <sstream>
#include <string>

namespace InvasionEngine {

Map::Map() : m_Width(0), m_Height(0) {
}

Map::~Map() {
}

bool Map::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Read map dimensions
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> m_Width >> m_Height;
    }

    // Read map data
    m_Tiles.resize(m_Width * m_Height);
    for (int y = 0; y < m_Height; ++y) {
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            for (int x = 0; x < m_Width; ++x) {
                int tileType;
                iss >> tileType;
                m_Tiles[y * m_Width + x] = static_cast<TileType>(tileType);
            }
        }
    }

    return true;
}

bool Map::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Write map dimensions
    file << m_Width << " " << m_Height << "\n";

    // Write map data
    for (int y = 0; y < m_Height; ++y) {
        for (int x = 0; x < m_Width; ++x) {
            file << static_cast<int>(m_Tiles[y * m_Width + x]) << " ";
        }
        file << "\n";
    }

    return true;
}

void Map::SetTile(int x, int y, TileType type) {
    if (x >= 0 && x < m_Width && y >= 0 && y < m_Height) {
        m_Tiles[y * m_Width + x] = type;
    }
}

Map::TileType Map::GetTile(int x, int y) const {
    if (x >= 0 && x < m_Width && y >= 0 && y < m_Height) {
        return m_Tiles[y * m_Width + x];
    }
    return TileType::Wall;
}

int Map::GetWidth() const {
    return m_Width;
}

int Map::GetHeight() const {
    return m_Height;
}

} // namespace InvasionEngine 