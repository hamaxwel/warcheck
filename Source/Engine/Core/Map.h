#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Math/Vector.h"
#include "Model.h"

namespace InvasionEngine {

struct MapObject {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    std::shared_ptr<Model> model;
};

struct SpawnPoint {
    Vector3 position;
    Vector3 rotation;
    bool isPlayerSpawn;
};

class Map {
public:
    Map(const std::string& filename);
    ~Map() = default;

    void Load();
    void Unload();
    void Draw() const;

    const std::vector<MapObject>& GetObjects() const { return m_Objects; }
    const std::vector<SpawnPoint>& GetSpawnPoints() const { return m_SpawnPoints; }
    const Vector3& GetPlayerSpawn() const;
    const std::vector<Vector3>& GetAlienSpawns() const { return m_AlienSpawns; }

private:
    void LoadObjects();
    void LoadSpawnPoints();
    void LoadCollisionData();

    std::string m_Filename;
    std::vector<MapObject> m_Objects;
    std::vector<SpawnPoint> m_SpawnPoints;
    std::vector<Vector3> m_AlienSpawns;
    Vector3 m_PlayerSpawn;
};

} // namespace InvasionEngine 