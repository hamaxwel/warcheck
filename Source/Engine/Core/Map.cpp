#include "Map.h"
#include <fstream>
#include <json/json.h>
#include <iostream>

namespace InvasionEngine {

Map::Map(const std::string& filename) : m_Filename(filename) {}

void Map::Load() {
    std::ifstream file(m_Filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << m_Filename << std::endl;
        return;
    }

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(file, root)) {
        std::cerr << "Failed to parse map file: " << reader.getFormattedErrorMessages() << std::endl;
        return;
    }

    // Load objects
    const Json::Value& objects = root["objects"];
    for (const auto& obj : objects) {
        MapObject mapObj;
        mapObj.position = Vector3(
            obj["position"][0].asFloat(),
            obj["position"][1].asFloat(),
            obj["position"][2].asFloat()
        );
        mapObj.rotation = Vector3(
            obj["rotation"][0].asFloat(),
            obj["rotation"][1].asFloat(),
            obj["rotation"][2].asFloat()
        );
        mapObj.scale = Vector3(
            obj["scale"][0].asFloat(),
            obj["scale"][1].asFloat(),
            obj["scale"][2].asFloat()
        );
        mapObj.model = std::make_shared<Model>(obj["model"].asString());
        m_Objects.push_back(mapObj);
    }

    // Load spawn points
    const Json::Value& spawns = root["spawns"];
    for (const auto& spawn : spawns) {
        SpawnPoint spawnPoint;
        spawnPoint.position = Vector3(
            spawn["position"][0].asFloat(),
            spawn["position"][1].asFloat(),
            spawn["position"][2].asFloat()
        );
        spawnPoint.rotation = Vector3(
            spawn["rotation"][0].asFloat(),
            spawn["rotation"][1].asFloat(),
            spawn["rotation"][2].asFloat()
        );
        spawnPoint.isPlayerSpawn = spawn["isPlayerSpawn"].asBool();
        
        if (spawnPoint.isPlayerSpawn) {
            m_PlayerSpawn = spawnPoint.position;
        } else {
            m_AlienSpawns.push_back(spawnPoint.position);
        }
        
        m_SpawnPoints.push_back(spawnPoint);
    }
}

void Map::Unload() {
    m_Objects.clear();
    m_SpawnPoints.clear();
    m_AlienSpawns.clear();
}

void Map::Draw() const {
    for (const auto& obj : m_Objects) {
        glPushMatrix();
        glTranslatef(obj.position.x, obj.position.y, obj.position.z);
        glRotatef(obj.rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(obj.rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(obj.rotation.z, 0.0f, 0.0f, 1.0f);
        glScalef(obj.scale.x, obj.scale.y, obj.scale.z);
        obj.model->Draw();
        glPopMatrix();
    }
}

const Vector3& Map::GetPlayerSpawn() const {
    return m_PlayerSpawn;
}

} // namespace InvasionEngine 