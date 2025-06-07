#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FInvasionCycleModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

// Game-specific constants
namespace InvasionCycleConstants
{
    // Faction types
    enum class EFactionType
    {
        Human,
        Vyrex
    };

    // Game modes
    enum class EGameMode
    {
        SinglePlayer,
        Coop,
        PvP
    };

    // Tactical view states
    enum class ETacticalViewState
    {
        FirstPerson,
        Tactical
    };

    // Core gameplay constants
    constexpr float DEFAULT_PLAYER_HEALTH = 100.0f;
    constexpr float DEFAULT_PLAYER_SPEED = 600.0f;
    constexpr int32 MAX_SQUAD_SIZE = 8;
    constexpr int32 MAX_PLAYERS_PVP = 16; // 8v8
    constexpr int32 MAX_PLAYERS_COOP = 2;
} 