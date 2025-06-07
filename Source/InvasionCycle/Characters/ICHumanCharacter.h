#pragma once

#include "CoreMinimal.h"
// #include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "ICHumanCharacter.generated.h"

UCLASS()
class INVASIONCYCLE_API ICHumanCharacter : public BaseCharacter
{
    GENERATED_BODY()

public:
    ICHumanCharacter();
    virtual ~ICHumanCharacter() override;
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
    virtual void BeginDestroy() override;

    // Shield management
    bool bIsShieldActive;
    float ShieldRechargeTimer;
    float TacticalScanTimer;

    // Helper functions
    void UpdateShield(float DeltaTime);
    void RechargeShield();
}; 