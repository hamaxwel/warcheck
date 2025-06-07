#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

#include "../Engine/Core/Entity.h"
#include "../Engine/Core/Component.h"
#include "../Engine/Math/Vector.h"
#include "../Engine/Math/Quaternion.h"
#include "../Engine/Math/Matrix.h"

namespace InvasionCycle {

// Forward declarations
class ICBaseCharacter;
class ICHumanCharacter;
class ICVyrexCharacter;
class ICGameMode;
class ICPlayerController;
class ICTacticalCommandManager;
class ICTacticalHUD;

// Constants
constexpr float DEFAULT_MOVEMENT_SPEED = 5.0f;
constexpr float DEFAULT_ROTATION_SPEED = 90.0f;
constexpr float DEFAULT_JUMP_FORCE = 5.0f;
constexpr float DEFAULT_GRAVITY = 9.81f;

// Enums
enum class ECharacterType {
    Human,
    Vyrex
};

enum class EWeaponType {
    Pistol,
    Rifle,
    Shotgun,
    Sniper,
    Melee
};

enum class ETacticalCommand {
    Move,
    Attack,
    Defend,
    Follow,
    Patrol
};

} // namespace InvasionCycle 