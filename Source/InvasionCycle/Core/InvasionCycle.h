#pragma once

#include <glm/glm.hpp>
#include <string>

namespace InvasionCycle {

// Faction types
enum class EFactionType {
    None,
    Human,
    Vyrex
};

// Weapon types
enum class EWeaponType {
    None,
    Pistol,
    Rifle,
    Shotgun,
    Sniper,
    Melee
};

// Character types
enum class ECharacterType {
    None,
    Soldier,
    Medic,
    Engineer,
    Scout,
    Heavy
};

// Game states
enum class EGameState {
    None,
    MainMenu,
    Loading,
    Playing,
    Paused,
    GameOver
};

// Constants
namespace Constants {
    constexpr float MaxHealth = 100.0f;
    constexpr float MaxArmor = 100.0f;
    constexpr float MaxSpeed = 10.0f;
    constexpr float MaxJumpHeight = 2.0f;
    constexpr float Gravity = -9.81f;
    constexpr float MouseSensitivity = 0.1f;
    constexpr float WeaponRange = 1000.0f;
    constexpr float WeaponDamage = 20.0f;
    constexpr float WeaponFireRate = 0.5f;
    constexpr float RespawnDelay = 5.0f;
    constexpr float RoundTime = 300.0f; // 5 minutes
}

// Forward declarations
class ICBaseCharacter;
class ICPlayerController;
class GameMode;

} // namespace InvasionCycle 