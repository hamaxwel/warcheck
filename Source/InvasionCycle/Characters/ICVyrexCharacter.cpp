#include "Characters/ICVyrexCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AICVyrexCharacter::AICVyrexCharacter()
{
    // Set default values for Vyrex-specific properties
    BiomassRegenerationRate = 5.0f;
    MaxBiomass = 100.0f;
    CurrentBiomass = MaxBiomass;
    CarapaceArmor = 0.2f; // 20% damage reduction
    AcidDamage = 25.0f;
    BurrowSpeed = 400.0f;
    BurrowCooldown = 8.0f;
    SpitRange = 1500.0f;
    SpitCooldown = 3.0f;

    bIsBurrowed = false;
    BurrowTimer = 0.0f;
    SpitTimer = 0.0f;

    // Set faction type
    FactionType = InvasionCycleConstants::EFactionType::Vyrex;
}

void AICVyrexCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AICVyrexCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update biomass and ability timers
    UpdateBiomass(DeltaTime);
    
    if (BurrowTimer > 0.0f)
    {
        BurrowTimer -= DeltaTime;
    }
    
    if (SpitTimer > 0.0f)
    {
        SpitTimer -= DeltaTime;
    }
}

void AICVyrexCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AICVyrexCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead)
        return;

    // Apply carapace armor reduction
    float ReducedDamage = DamageAmount * (1.0f - CarapaceArmor);
    
    // If burrowed, take reduced damage
    if (bIsBurrowed)
    {
        ReducedDamage *= 0.5f;
    }

    Super::TakeDamage(ReducedDamage, DamageEvent, EventInstigator, DamageCauser);
}

void AICVyrexCharacter::OnHealthChanged()
{
    Super::OnHealthChanged();
    
    // Add Vyrex-specific health change effects here
    // (e.g., biomass particle effects, sound effects)
}

void AICVyrexCharacter::Burrow()
{
    if (!bIsBurrowed && BurrowTimer <= 0.0f)
    {
        bIsBurrowed = true;
        
        // Modify movement properties
        GetCharacterMovement()->MaxWalkSpeed = BurrowSpeed;
        GetCharacterMovement()->bOrientRotationToMovement = false;
        
        // Add burrow effects here
        // (e.g., particle effects, sound effects)
        
        // Start cooldown
        BurrowTimer = BurrowCooldown;
    }
}

void AICVyrexCharacter::Unburrow()
{
    if (bIsBurrowed)
    {
        bIsBurrowed = false;
        
        // Restore movement properties
        GetCharacterMovement()->MaxWalkSpeed = InvasionCycleConstants::DEFAULT_PLAYER_SPEED;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        
        // Add unburrow effects here
    }
}

void AICVyrexCharacter::SpitAcid(const FVector& TargetLocation)
{
    if (SpitTimer <= 0.0f && CurrentBiomass >= 20.0f)
    {
        // Calculate direction to target
        FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
        
        // Perform line trace to check for obstacles
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            GetActorLocation(),
            GetActorLocation() + Direction * SpitRange,
            ECC_Visibility,
            QueryParams
        );
        
        if (bHit)
        {
            // Apply acid damage to hit actor
            if (AActor* HitActor = HitResult.GetActor())
            {
                UGameplayStatics::ApplyDamage(
                    HitActor,
                    AcidDamage,
                    GetController(),
                    this,
                    UDamageType::StaticClass()
                );
                
                // Add acid effect at impact point
                // (e.g., particle effects, decals)
            }
        }
        
        // Consume biomass
        CurrentBiomass = FMath::Max(0.0f, CurrentBiomass - 20.0f);
        
        // Start cooldown
        SpitTimer = SpitCooldown;
    }
}

void AICVyrexCharacter::UpdateBiomass(float DeltaTime)
{
    if (CurrentBiomass < MaxBiomass)
    {
        RegenerateBiomass();
    }
}

void AICVyrexCharacter::RegenerateBiomass()
{
    CurrentBiomass = FMath::Min(MaxBiomass, CurrentBiomass + BiomassRegenerationRate * GetWorld()->GetDeltaSeconds());
} 