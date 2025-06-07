#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Characters/ICBaseCharacter.h"
#include "Tactical/ICTacticalCommandInterface.h"
#include "ICTacticalHUD.generated.h"

USTRUCT(BlueprintType)
struct FSquadMemberInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite)
    float Health;

    UPROPERTY(BlueprintReadWrite)
    float Shield;

    UPROPERTY(BlueprintReadWrite)
    bool bIsLeader;

    UPROPERTY(BlueprintReadWrite)
    EFactionType FactionType;
};

UCLASS()
class INVASIONCYCLE_API AICTacticalHUD : public AHUD
{
    GENERATED_BODY()

public:
    AICTacticalHUD();

    virtual void DrawHUD() override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void UpdateSquadInfo(const TArray<FSquadMemberInfo>& SquadMembers);

    UFUNCTION(BlueprintCallable)
    void ShowCommandFeedback(const FVector& Location, ECommandType CommandType);

    UFUNCTION(BlueprintCallable)
    void ShowSquadJoinPrompt(APlayerController* SquadLeader);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> TacticalWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> SquadJoinWidgetClass;

    UPROPERTY()
    UUserWidget* TacticalWidget;

    UPROPERTY()
    UUserWidget* SquadJoinWidget;

    UPROPERTY()
    TArray<FSquadMemberInfo> CurrentSquadInfo;

    UPROPERTY()
    TArray<FVector> CommandFeedbackLocations;

    UPROPERTY()
    TArray<ECommandType> CommandFeedbackTypes;

    UPROPERTY()
    TArray<float> CommandFeedbackTimers;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    float CommandFeedbackDuration;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    FLinearColor MoveCommandColor;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    FLinearColor AttackCommandColor;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    FLinearColor DefendCommandColor;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    FLinearColor SpecialCommandColor;

    void DrawCommandFeedback();
    void UpdateCommandFeedback(float DeltaTime);
}; 