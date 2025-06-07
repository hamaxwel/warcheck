#pragma once

#include "CoreMinimal.h"
// #include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "Tactical/ICTacticalCommandInterface.h"
#include "InvasionCycle.h"
#include "ICBaseCharacter.generated.h"

class ICBaseCharacter : public BaseCharacter
{
    GENERATED_BODY()
public:
    ICBaseCharacter();
    virtual ~ICBaseCharacter() override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Faction and team management
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    InvasionCycleConstants::EFactionType FactionType;

    // Health and damage system
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float CurrentHealth;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

    // Tactical command interface implementation
    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void ExecuteCommand(const FCommandData& Command);
    virtual void ExecuteCommand_Implementation(const FCommandData& Command) override;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void CancelCommand();
    virtual void CancelCommand_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    bool CanExecuteCommand(const FCommandData& Command) const;
    virtual bool CanExecuteCommand_Implementation(const FCommandData& Command) const override;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void JoinSquad(AActor* SquadLeader);
    virtual void JoinSquad_Implementation(AActor* SquadLeader) override;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    void LeaveSquad();
    virtual void LeaveSquad_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    FString GetStatusReport() const;
    virtual FString GetStatusReport_Implementation() const override;

    UFUNCTION(BlueprintNativeEvent, Category = "Tactical")
    bool IsCommandComplete() const;
    virtual bool IsCommandComplete_Implementation() const override;

    // Movement and abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeedMultiplier;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void StopSprint();

protected:
    // Character state
    bool bIsSprinting;
    bool bIsDead;

    // Command state
    FCommandData CurrentCommand;
    bool bHasActiveCommand;
    AActor* SquadLeader;

    // Helper functions
    virtual void Die();
    virtual void OnHealthChanged();
    virtual void ProcessCommand(const FCommandData& Command);

    virtual void BeginDestroy() override;
}; 