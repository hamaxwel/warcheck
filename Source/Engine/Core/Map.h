#pragma once
#include <vector>
#include <string>

namespace InvasionEngine {

class Map {
public:
    enum class TileType {
        Empty,
        Wall,
        Floor,
        Door,
        Window
    };

    Map();
    ~Map();

    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename) const;

    void SetTile(int x, int y, TileType type);
    TileType GetTile(int x, int y) const;

    int GetWidth() const;
    int GetHeight() const;

private:
    int m_Width;
    int m_Height;
    std::vector<TileType> m_Tiles;
};

} // namespace InvasionEngine 