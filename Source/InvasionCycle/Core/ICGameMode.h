#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InvasionCycle.h"
#include "ICGameMode.generated.h"

UCLASS()
class INVASIONCYCLE_API AICGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AICGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Game state management
    UFUNCTION(BlueprintCallable, Category = "Game")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void EndGame();

    // Player management
    UFUNCTION(BlueprintCallable, Category = "Game|Player")
    void SpawnPlayer(APlayerController* PlayerController, InvasionCycleConstants::EFactionType Faction);

    UFUNCTION(BlueprintCallable, Category = "Game|Player")
    void RespawnPlayer(APlayerController* PlayerController);

    // Squad management
    UFUNCTION(BlueprintCallable, Category = "Game|Squad")
    void CreateSquad(APlayerController* Leader, const FString& SquadName);

    UFUNCTION(BlueprintCallable, Category = "Game|Squad")
    void AddToSquad(APlayerController* Member, APlayerController* SquadLeader);

    // Game rules
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Rules")
    float RespawnDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Rules")
    int32 MaxSquadSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Rules")
    float RoundTime;

    // Game state
    UPROPERTY(BlueprintReadOnly, Category = "Game|State")
    bool bIsGameActive;

    UPROPERTY(BlueprintReadOnly, Category = "Game|State")
    float RemainingRoundTime;

    UPROPERTY(BlueprintReadOnly, Category = "Game|State")
    TMap<InvasionCycleConstants::EFactionType, int32> FactionScores;

protected:
    // Spawn points
    UPROPERTY(EditAnywhere, Category = "Game|Spawn")
    TArray<class ATargetPoint*> HumanSpawnPoints;

    UPROPERTY(EditAnywhere, Category = "Game|Spawn")
    TArray<class ATargetPoint*> VyrexSpawnPoints;

    // Game state tracking
    UPROPERTY()
    TMap<APlayerController*, float> RespawnTimers;

    UPROPERTY()
    TMap<APlayerController*, APlayerController*> SquadLeaders;

    // Helper functions
    void InitializeGame();
    void UpdateGameState(float DeltaTime);
    void ProcessRespawns(float DeltaTime);
    ATargetPoint* GetSpawnPoint(InvasionCycleConstants::EFactionType Faction);
    void UpdateScores();
}; 