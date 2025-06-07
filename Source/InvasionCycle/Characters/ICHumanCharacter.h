#pragma once

#include "CoreMinimal.h"
#include "Characters/ICBaseCharacter.h"
#include "ICHumanCharacter.generated.h"

UCLASS()
class INVASIONCYCLE_API AICHumanCharacter : public AICBaseCharacter
{
    GENERATED_BODY()

public:
    AICHumanCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Human-specific properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
    float ShieldCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
    float CurrentShield;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
    float ShieldRechargeRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
    float ShieldRechargeDelay;

    // Tactical abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human|Tactical")
    float TacticalScanRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human|Tactical")
    float TacticalScanCooldown;

    // Combat abilities
    UFUNCTION(BlueprintCallable, Category = "Human|Combat")
    void ActivateShield();

    UFUNCTION(BlueprintCallable, Category = "Human|Combat")
    void DeactivateShield();

    UFUNCTION(BlueprintCallable, Category = "Human|Tactical")
    void PerformTacticalScan();

    // Override base functions
    virtual void TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    virtual void OnHealthChanged() override;

protected:
    // Shield management
    bool bIsShieldActive;
    float ShieldRechargeTimer;
    float TacticalScanTimer;

    // Helper functions
    void UpdateShield(float DeltaTime);
    void RechargeShield();
}; 