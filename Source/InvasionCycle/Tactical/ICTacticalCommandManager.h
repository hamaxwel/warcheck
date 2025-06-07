#pragma once

// #include "GameFramework/Actor.h"
#include "Tactical/ICTacticalCommandInterface.h"
#include "ICTacticalCommandManager.generated.h"

UENUM(BlueprintType)
enum class ESquadFormation : uint8
{
    Line,
    Column,
    Wedge,
    Echelon,
    Diamond
};

UENUM(BlueprintType)
enum class ESquadRole : uint8
{
    Leader,
    Point,
    Flank,
    Rear,
    Support
};

UCLASS()
class INVASIONCYCLE_API AICTacticalCommandManager : public AActor
{
    GENERATED_BODY()

public:
    AICTacticalCommandManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Squad management
    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    void CreateSquad(AActor* Leader);

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    void AddToSquad(AActor* Member, AActor* SquadLeader);

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    void RemoveFromSquad(AActor* Member);

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    void SetSquadFormation(AActor* SquadLeader, ESquadFormation NewFormation);

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    void AssignSquadRole(AActor* Member, ESquadRole NewRole);

    // Command execution
    UFUNCTION(BlueprintCallable, Category = "Tactical|Command")
    void IssueCommand(const FCommandData& Command, AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Tactical|Command")
    void IssueSquadCommand(const FCommandData& Command, AActor* SquadLeader);

    // Command validation and processing
    UFUNCTION(BlueprintCallable, Category = "Tactical|Command")
    bool ValidateCommand(const FCommandData& Command, AActor* Target) const;

    UFUNCTION(BlueprintCallable, Category = "Tactical|Command")
    void ProcessCommandQueue();

    // Squad status and information
    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    TArray<AActor*> GetSquadMembers(AActor* SquadLeader) const;

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    bool IsInSquad(AActor* Member) const;

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    ESquadFormation GetSquadFormation(AActor* SquadLeader) const;

    UFUNCTION(BlueprintCallable, Category = "Tactical|Squad")
    ESquadRole GetSquadRole(AActor* Member) const;

protected:
    // Squad data structure
    USTRUCT()
    struct FSquadMemberData
    {
        GENERATED_BODY()

        UPROPERTY()
        AActor* Member;

        UPROPERTY()
        ESquadRole Role;

        UPROPERTY()
        FVector FormationOffset;
    };

    USTRUCT()
    struct FSquadData
    {
        GENERATED_BODY()

        UPROPERTY()
        AActor* Leader;

        UPROPERTY()
        TArray<FSquadMemberData> Members;

        UPROPERTY()
        TArray<FCommandData> CommandQueue;

        UPROPERTY()
        ESquadFormation Formation;

        UPROPERTY()
        float FormationSpacing;
    };

    // Squad management
    UPROPERTY()
    TMap<AActor*, FSquadData> Squads;

    // Command processing
    UPROPERTY()
    TMap<AActor*, TArray<FCommandData>> CommandQueues;

    // Formation settings
    UPROPERTY(EditDefaultsOnly, Category = "Tactical|Formation")
    float DefaultFormationSpacing;

    // Helper functions
    void InitializeSquad(AActor* Leader);
    void CleanupSquad(AActor* SquadLeader);
    void UpdateSquadStatus(AActor* SquadLeader);
    bool CanJoinSquad(AActor* Member, AActor* SquadLeader) const;
    void UpdateFormationPositions(AActor* SquadLeader);
    FVector CalculateFormationOffset(const FSquadData& Squad, const FSquadMemberData& Member) const;
}; 