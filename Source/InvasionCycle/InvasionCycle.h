#pragma once

#include <Engine/Core/GameMode.h>
#include <Engine/Core/Character.h>
#include <glm/glm.hpp>

namespace InvasionCycle {

// Game-specific constants
namespace Constants {
    // Faction types
    enum class FactionType {
        Human,
        Vyrex
    };

    // Game modes
    enum class GameModeType {
        SinglePlayer,
        Coop,
        PvP
    };

    // Tactical view states
    enum class TacticalViewState {
        FirstPerson,
        Tactical
    };

    // Core gameplay constants
    constexpr float DEFAULT_PLAYER_HEALTH = 100.0f;
    constexpr float DEFAULT_PLAYER_SPEED = 600.0f;
    constexpr int MAX_SQUAD_SIZE = 8;
    constexpr int MAX_PLAYERS_PVP = 16; // 8v8
    constexpr int MAX_PLAYERS_COOP = 4;
}

// Forward declarations
class HumanCharacter;
class VyrexCharacter;
class GameMode;

} // namespace InvasionCycle 