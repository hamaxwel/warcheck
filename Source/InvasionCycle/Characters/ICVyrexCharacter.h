#pragma once

#include "CoreMinimal.h"
// #include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "ICVyrexCharacter.generated.h"

UCLASS()
class INVASIONCYCLE_API ICVyrexCharacter : public BaseCharacter
{
    GENERATED_BODY()

public:
    ICVyrexCharacter();
    virtual ~ICVyrexCharacter() override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Vyrex-specific properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex")
    float BiomassRegenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex")
    float CurrentBiomass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex")
    float MaxBiomass;

    // Evolution traits
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex|Evolution")
    float CarapaceArmor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex|Evolution")
    float AcidDamage;

    // Special abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex|Abilities")
    float BurrowSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex|Abilities")
    float BurrowCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex|Abilities")
    float SpitRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyrex|Abilities")
    float SpitCooldown;

    // Combat abilities
    UFUNCTION(BlueprintCallable, Category = "Vyrex|Combat")
    void Burrow();

    UFUNCTION(BlueprintCallable, Category = "Vyrex|Combat")
    void Unburrow();

    UFUNCTION(BlueprintCallable, Category = "Vyrex|Combat")
    void SpitAcid(const FVector& TargetLocation);

    // Override base functions
    virtual void TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    virtual void OnHealthChanged() override;

protected:
    // State tracking
    bool bIsBurrowed;
    float BurrowTimer;
    float SpitTimer;

    // Helper functions
    void UpdateBiomass(float DeltaTime);
    void RegenerateBiomass();

    virtual void BeginDestroy() override;
}; 