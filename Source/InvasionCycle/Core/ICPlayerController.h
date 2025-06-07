#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InvasionCycle.h"
#include "ICPlayerController.generated.h"

UCLASS()
class INVASIONCYCLE_API AICPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AICPlayerController();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // View switching
    UFUNCTION(BlueprintCallable, Category = "View")
    void SwitchToTacticalView();

    UFUNCTION(BlueprintCallable, Category = "View")
    void SwitchToFirstPersonView();

    // Command input
    UFUNCTION(BlueprintCallable, Category = "Command")
    void IssueMoveCommand(const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void IssueAttackCommand(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void IssueDefendCommand(const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void IssueSpecialCommand(const FVector& Location);

    // Squad management
    UFUNCTION(BlueprintCallable, Category = "Squad")
    void CreateSquad();

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void JoinSquad(APlayerController* SquadLeader);

    // Camera settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float TacticalViewHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float TacticalViewPitch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float TacticalViewTransitionSpeed;

protected:
    // Input functions
    void OnTacticalViewPressed();
    void OnTacticalViewReleased();
    void OnCommandPressed();
    void OnCommandReleased();
    void OnSquadCreatePressed();
    void OnSquadJoinPressed();

    // Camera management
    UPROPERTY()
    class UCameraComponent* TacticalCamera;

    UPROPERTY()
    class USpringArmComponent* CameraBoom;

    // View state
    UPROPERTY()
    bool bIsInTacticalView;

    UPROPERTY()
    bool bIsCommanding;

    // Helper functions
    void InitializeCamera();
    void UpdateCamera(float DeltaTime);
    void ProcessCommandInput();
    FVector GetMouseWorldPosition() const;
}; 