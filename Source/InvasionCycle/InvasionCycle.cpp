#include "InvasionCycle.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FInvasionCycleModule, InvasionCycle, "InvasionCycle");

void FInvasionCycleModule::StartupModule()
{
    // Initialize core systems
    UE_LOG(LogTemp, Log, TEXT("InvasionCycle module starting up"));
}

void FInvasionCycleModule::ShutdownModule()
{
    // Clean up resources
    UE_LOG(LogTemp, Log, TEXT("InvasionCycle module shutting down"));
} 