#include "Core/ICGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ICHumanCharacter.h"
#include "Characters/ICVyrexCharacter.h"
#include "Tactical/ICTacticalCommandManager.h"

AICGameMode::AICGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    RespawnDelay = 5.0f;
    MaxSquadSize = 8;
    RoundTime = 600.0f; // 10 minutes
    bIsGameActive = false;
    RemainingRoundTime = RoundTime;

    // Initialize faction scores
    FactionScores.Add(InvasionCycleConstants::EFactionType::Human, 0);
    FactionScores.Add(InvasionCycleConstants::EFactionType::Vyrex, 0);
}

void AICGameMode::BeginPlay()
{
    Super::BeginPlay();
    InitializeGame();
}

void AICGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsGameActive)
    {
        UpdateGameState(DeltaTime);
        ProcessRespawns(DeltaTime);
    }
}

void AICGameMode::StartGame()
{
    if (!bIsGameActive)
    {
        bIsGameActive = true;
        RemainingRoundTime = RoundTime;
        
        // Reset scores
        FactionScores[InvasionCycleConstants::EFactionType::Human] = 0;
        FactionScores[InvasionCycleConstants::EFactionType::Vyrex] = 0;

        // Spawn all players
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PlayerController = It->Get();
            if (PlayerController)
            {
                // Determine faction based on team assignment
                InvasionCycleConstants::EFactionType Faction = 
                    (PlayerController->GetTeamNum() == 0) ? 
                    InvasionCycleConstants::EFactionType::Human : 
                    InvasionCycleConstants::EFactionType::Vyrex;

                SpawnPlayer(PlayerController, Faction);
            }
        }
    }
}

void AICGameMode::EndGame()
{
    if (bIsGameActive)
    {
        bIsGameActive = false;
        
        // Determine winner
        int32 HumanScore = FactionScores[InvasionCycleConstants::EFactionType::Human];
        int32 VyrexScore = FactionScores[InvasionCycleConstants::EFactionType::Vyrex];
        
        // Broadcast game end event
        // TODO: Implement game end UI and rewards
    }
}

void AICGameMode::SpawnPlayer(APlayerController* PlayerController, InvasionCycleConstants::EFactionType Faction)
{
    if (!PlayerController)
        return;

    // Get spawn point
    ATargetPoint* SpawnPoint = GetSpawnPoint(Faction);
    if (!SpawnPoint)
        return;

    // Spawn appropriate character
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AICBaseCharacter* Character = nullptr;
    if (Faction == InvasionCycleConstants::EFactionType::Human)
    {
        Character = GetWorld()->SpawnActor<AICHumanCharacter>(
            AICHumanCharacter::StaticClass(),
            SpawnPoint->GetActorLocation(),
            SpawnPoint->GetActorRotation(),
            SpawnParams
        );
    }
    else
    {
        Character = GetWorld()->SpawnActor<AICVyrexCharacter>(
            AICVyrexCharacter::StaticClass(),
            SpawnPoint->GetActorLocation(),
            SpawnPoint->GetActorRotation(),
            SpawnParams
        );
    }

    if (Character)
    {
        PlayerController->Possess(Character);
    }
}

void AICGameMode::RespawnPlayer(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        RespawnTimers.Add(PlayerController, RespawnDelay);
    }
}

void AICGameMode::CreateSquad(APlayerController* Leader, const FString& SquadName)
{
    if (Leader && !SquadLeaders.Contains(Leader))
    {
        SquadLeaders.Add(Leader, Leader);
        
        // Create tactical command manager if it doesn't exist
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
        
        AICTacticalCommandManager* CommandManager = nullptr;
        if (FoundActors.Num() > 0)
        {
            CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
        }
        else
        {
            FActorSpawnParameters SpawnParams;
            CommandManager = GetWorld()->SpawnActor<AICTacticalCommandManager>(
                AICTacticalCommandManager::StaticClass(),
                FVector::ZeroVector,
                FRotator::ZeroRotator,
                SpawnParams
            );
        }

        if (CommandManager)
        {
            CommandManager->CreateSquad(Leader->GetPawn());
        }
    }
}

void AICGameMode::AddToSquad(APlayerController* Member, APlayerController* SquadLeader)
{
    if (Member && SquadLeader && SquadLeaders.Contains(SquadLeader))
    {
        // Find the tactical command manager
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
        
        if (FoundActors.Num() > 0)
        {
            AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
            if (CommandManager)
            {
                CommandManager->AddToSquad(Member->GetPawn(), SquadLeader->GetPawn());
            }
        }
    }
}

void AICGameMode::InitializeGame()
{
    // Find all spawn points
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        ATargetPoint* SpawnPoint = Cast<ATargetPoint>(Actor);
        if (SpawnPoint)
        {
            // Categorize spawn points based on their tags
            if (SpawnPoint->ActorHasTag(TEXT("Human")))
            {
                HumanSpawnPoints.Add(SpawnPoint);
            }
            else if (SpawnPoint->ActorHasTag(TEXT("Vyrex")))
            {
                VyrexSpawnPoints.Add(SpawnPoint);
            }
        }
    }
}

void AICGameMode::UpdateGameState(float DeltaTime)
{
    // Update round timer
    if (RemainingRoundTime > 0.0f)
    {
        RemainingRoundTime -= DeltaTime;
        if (RemainingRoundTime <= 0.0f)
        {
            EndGame();
        }
    }

    // Update scores
    UpdateScores();
}

void AICGameMode::ProcessRespawns(float DeltaTime)
{
    TArray<APlayerController*> PlayersToRespawn;

    // Update respawn timers
    for (auto& RespawnPair : RespawnTimers)
    {
        RespawnPair.Value -= DeltaTime;
        if (RespawnPair.Value <= 0.0f)
        {
            PlayersToRespawn.Add(RespawnPair.Key);
        }
    }

    // Respawn players
    for (APlayerController* PlayerController : PlayersToRespawn)
    {
        if (PlayerController)
        {
            // Determine faction based on team assignment
            InvasionCycleConstants::EFactionType Faction = 
                (PlayerController->GetTeamNum() == 0) ? 
                InvasionCycleConstants::EFactionType::Human : 
                InvasionCycleConstants::EFactionType::Vyrex;

            SpawnPlayer(PlayerController, Faction);
        }
        RespawnTimers.Remove(PlayerController);
    }
}

ATargetPoint* AICGameMode::GetSpawnPoint(InvasionCycleConstants::EFactionType Faction)
{
    TArray<ATargetPoint*>& SpawnPoints = 
        (Faction == InvasionCycleConstants::EFactionType::Human) ? 
        HumanSpawnPoints : VyrexSpawnPoints;

    if (SpawnPoints.Num() > 0)
    {
        // Get a random spawn point
        int32 Index = FMath::RandRange(0, SpawnPoints.Num() - 1);
        return SpawnPoints[Index];
    }

    return nullptr;
}

void AICGameMode::UpdateScores()
{
    // Reset scores
    FactionScores[InvasionCycleConstants::EFactionType::Human] = 0;
    FactionScores[InvasionCycleConstants::EFactionType::Vyrex] = 0;

    // Count living players for each faction
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = It->Get();
        if (PlayerController && PlayerController->GetPawn())
        {
            AICBaseCharacter* Character = Cast<AICBaseCharacter>(PlayerController->GetPawn());
            if (Character && !Character->IsDead())
            {
                FactionScores[Character->FactionType]++;
            }
        }
    }
} 