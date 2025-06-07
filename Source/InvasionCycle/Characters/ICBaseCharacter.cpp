#include "Characters/ICBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

AICBaseCharacter::AICBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    MaxHealth = InvasionCycleConstants::DEFAULT_PLAYER_HEALTH;
    CurrentHealth = MaxHealth;
    SprintSpeedMultiplier = 1.5f;
    bIsSprinting = false;
    bIsDead = false;

    // Initialize command state
    bHasActiveCommand = false;
    SquadLeader = nullptr;
}

void AICBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AICBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AICBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AICBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead)
        return;

    CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
    OnHealthChanged();

    if (CurrentHealth <= 0.0f)
    {
        Die();
    }
}

void AICBaseCharacter::ExecuteCommand_Implementation(const FCommandData& Command)
{
    if (CanExecuteCommand_Implementation(Command))
    {
        CurrentCommand = Command;
        bHasActiveCommand = true;
        ProcessCommand(Command);
    }
}

void AICBaseCharacter::CancelCommand_Implementation()
{
    if (bHasActiveCommand)
    {
        bHasActiveCommand = false;
        // Add command cancellation effects here
    }
}

bool AICBaseCharacter::CanExecuteCommand_Implementation(const FCommandData& Command) const
{
    if (bIsDead)
        return false;

    // Check if the command is valid for this character
    switch (Command.Type)
    {
        case ITacticalCommandInterface::ECommandType::Move:
            return true;
        case ITacticalCommandInterface::ECommandType::Attack:
            return Command.TargetActor != nullptr;
        case ITacticalCommandInterface::ECommandType::Defend:
            return Command.TargetLocation != FVector::ZeroVector;
        case ITacticalCommandInterface::ECommandType::Special:
            return true; // Override in derived classes
        case ITacticalCommandInterface::ECommandType::Follow:
            return Command.TargetActor != nullptr;
        default:
            return false;
    }
}

void AICBaseCharacter::JoinSquad_Implementation(AActor* NewSquadLeader)
{
    if (NewSquadLeader && NewSquadLeader != this)
    {
        SquadLeader = NewSquadLeader;
        // Add squad join effects here
    }
}

void AICBaseCharacter::LeaveSquad_Implementation()
{
    if (SquadLeader)
    {
        SquadLeader = nullptr;
        // Add squad leave effects here
    }
}

FString AICBaseCharacter::GetStatusReport_Implementation() const
{
    FString Status = FString::Printf(TEXT("Health: %.0f/%.0f"), CurrentHealth, MaxHealth);
    
    if (bHasActiveCommand)
    {
        Status += FString::Printf(TEXT("\nCurrent Command: %s"), 
            *UEnum::GetValueAsString(CurrentCommand.Type));
    }
    
    if (SquadLeader)
    {
        Status += TEXT("\nIn Squad");
    }
    
    return Status;
}

bool AICBaseCharacter::IsCommandComplete_Implementation() const
{
    if (!bHasActiveCommand)
        return true;

    // Check if the current command has been completed
    switch (CurrentCommand.Type)
    {
        case ITacticalCommandInterface::ECommandType::Move:
            return FVector::Distance(GetActorLocation(), CurrentCommand.TargetLocation) < 100.0f;
        case ITacticalCommandInterface::ECommandType::Attack:
            return !IsValid(CurrentCommand.TargetActor);
        case ITacticalCommandInterface::ECommandType::Defend:
            return false; // Defend commands are ongoing
        case ITacticalCommandInterface::ECommandType::Special:
            return true; // Override in derived classes
        case ITacticalCommandInterface::ECommandType::Follow:
            return !IsValid(CurrentCommand.TargetActor);
        default:
            return true;
    }
}

void AICBaseCharacter::StartSprint()
{
    if (!bIsSprinting)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
    }
}

void AICBaseCharacter::StopSprint()
{
    if (bIsSprinting)
    {
        bIsSprinting = false;
        GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
    }
}

void AICBaseCharacter::Die()
{
    if (!bIsDead)
    {
        bIsDead = true;
        // Disable character movement and collision
        GetCharacterMovement()->DisableMovement();
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        // Cancel any active commands
        CancelCommand_Implementation();
        
        // Leave squad if in one
        if (SquadLeader)
        {
            LeaveSquad_Implementation();
        }
        
        // Notify game mode of death
        if (AController* Controller = GetController())
        {
            Controller->UnPossess();
        }
    }
}

void AICBaseCharacter::OnHealthChanged()
{
    // Base implementation - can be overridden by derived classes
    // for faction-specific health change effects
}

void AICBaseCharacter::ProcessCommand(const FCommandData& Command)
{
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        switch (Command.Type)
        {
            case ITacticalCommandInterface::ECommandType::Move:
            {
                // Find tactical command manager
                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
                
                if (FoundActors.Num() > 0)
                {
                    AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
                    if (CommandManager && SquadLeader)
                    {
                        // Get formation offset
                        FVector FormationOffset = FVector::ZeroVector;
                        for (const auto& SquadPair : CommandManager->Squads)
                        {
                            if (SquadPair.Key == SquadLeader)
                            {
                                for (const auto& MemberData : SquadPair.Value.Members)
                                {
                                    if (MemberData.Member == this)
                                    {
                                        FormationOffset = MemberData.FormationOffset;
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        // Calculate target position with formation offset
                        FVector TargetPosition = Command.TargetLocation + FormationOffset;
                        AIController->MoveToLocation(TargetPosition);
                    }
                    else
                    {
                        // No squad, move directly to target
                        AIController->MoveToLocation(Command.TargetLocation);
                    }
                }
                break;
            }
            case ITacticalCommandInterface::ECommandType::Attack:
                if (Command.TargetActor)
                {
                    AIController->MoveToActor(Command.TargetActor);
                }
                break;
            case ITacticalCommandInterface::ECommandType::Defend:
                AIController->MoveToLocation(Command.TargetLocation);
                break;
            case ITacticalCommandInterface::ECommandType::Follow:
                if (Command.TargetActor)
                {
                    AIController->MoveToActor(Command.TargetActor);
                }
                break;
            case ITacticalCommandInterface::ECommandType::Special:
                // Override in derived classes
                break;
        }
    }
} 