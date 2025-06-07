#include "Characters/ICHumanCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AICHumanCharacter::AICHumanCharacter()
{
    // Set default values for human-specific properties
    ShieldCapacity = 50.0f;
    CurrentShield = ShieldCapacity;
    ShieldRechargeRate = 10.0f;
    ShieldRechargeDelay = 3.0f;
    TacticalScanRadius = 1000.0f;
    TacticalScanCooldown = 5.0f;

    bIsShieldActive = false;
    ShieldRechargeTimer = 0.0f;
    TacticalScanTimer = 0.0f;

    // Set faction type
    FactionType = InvasionCycleConstants::EFactionType::Human;
}

void AICHumanCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AICHumanCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update shield and ability timers
    UpdateShield(DeltaTime);
    
    if (TacticalScanTimer > 0.0f)
    {
        TacticalScanTimer -= DeltaTime;
    }
}

void AICHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AICHumanCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead)
        return;

    float RemainingDamage = DamageAmount;

    // Shield absorbs damage first if active
    if (bIsShieldActive && CurrentShield > 0.0f)
    {
        if (CurrentShield >= RemainingDamage)
        {
            CurrentShield -= RemainingDamage;
            RemainingDamage = 0.0f;
        }
        else
        {
            RemainingDamage -= CurrentShield;
            CurrentShield = 0.0f;
        }
    }

    // Apply remaining damage to health
    if (RemainingDamage > 0.0f)
    {
        Super::TakeDamage(RemainingDamage, DamageEvent, EventInstigator, DamageCauser);
    }

    // Start shield recharge delay
    ShieldRechargeTimer = ShieldRechargeDelay;
}

void AICHumanCharacter::OnHealthChanged()
{
    Super::OnHealthChanged();
    
    // Add human-specific health change effects here
    // (e.g., screen effects, sound effects, etc.)
}

void AICHumanCharacter::ActivateShield()
{
    if (!bIsShieldActive && CurrentShield > 0.0f)
    {
        bIsShieldActive = true;
        // Add shield activation effects here
    }
}

void AICHumanCharacter::DeactivateShield()
{
    if (bIsShieldActive)
    {
        bIsShieldActive = false;
        // Add shield deactivation effects here
    }
}

void AICHumanCharacter::PerformTacticalScan()
{
    if (TacticalScanTimer <= 0.0f)
    {
        // Perform sphere trace for enemies
        TArray<FHitResult> HitResults;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(TacticalScanRadius);
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        bool bHit = GetWorld()->SweepMultiByChannel(
            HitResults,
            GetActorLocation(),
            GetActorLocation(),
            FQuat::Identity,
            ECC_Pawn,
            Sphere,
            QueryParams
        );

        // Visualize scan radius
        DrawDebugSphere(
            GetWorld(),
            GetActorLocation(),
            TacticalScanRadius,
            32,
            FColor::Blue,
            false,
            1.0f
        );

        // Process scan results
        for (const FHitResult& Hit : HitResults)
        {
            if (AActor* HitActor = Hit.GetActor())
            {
                // Add enemy detection logic here
                // (e.g., mark enemies on minimap, update tactical view)
            }
        }

        // Reset cooldown
        TacticalScanTimer = TacticalScanCooldown;
    }
}

void AICHumanCharacter::UpdateShield(float DeltaTime)
{
    if (!bIsShieldActive && CurrentShield < ShieldCapacity)
    {
        if (ShieldRechargeTimer > 0.0f)
        {
            ShieldRechargeTimer -= DeltaTime;
        }
        else
        {
            RechargeShield();
        }
    }
}

void AICHumanCharacter::RechargeShield()
{
    CurrentShield = FMath::Min(ShieldCapacity, CurrentShield + ShieldRechargeRate * GetWorld()->GetDeltaSeconds());
} 