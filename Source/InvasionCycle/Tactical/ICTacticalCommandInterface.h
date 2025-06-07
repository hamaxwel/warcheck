#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICTacticalCommandInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTacticalCommandInterface : public UInterface
{
    GENERATED_BODY()
};

class INVASIONCYCLE_API ITacticalCommandInterface
{
    GENERATED_BODY()

public:
    // Command types that can be issued
    UENUM(BlueprintType)
    enum class ECommandType : uint8
    {
        Move,
        Attack,
        Defend,
        Special,
        Follow
    };

    // Command structure containing all necessary information
    USTRUCT(BlueprintType)
    struct FCommandData
    {
        GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        ECommandType Type;

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector TargetLocation;

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        AActor* TargetActor;

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Priority;

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsUrgent;
    };

    // Core tactical command functions
    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void ExecuteCommand(const FCommandData& Command);

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void CancelCommand();

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    bool CanExecuteCommand(const FCommandData& Command) const;

    // Squad management
    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void JoinSquad(AActor* SquadLeader);

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void LeaveSquad();

    // Status reporting
    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    FString GetStatusReport() const;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    bool IsCommandComplete() const;
}; 