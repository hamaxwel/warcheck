#include "Core/ICGameMode.h"
#include "Characters/ICHumanCharacter.h"
#include "Characters/ICVyrexCharacter.h"
#include "Tactical/ICTacticalCommandManager.h"
#include "ICPlayerController.h"
#include "ICBaseCharacter.h"
#include <algorithm>

namespace InvasionCycle {

GameMode::GameMode()
    : m_IsGameActive(false)
    , m_RemainingRoundTime(RoundTime)
{
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    RespawnDelay = 5.0f;
    MaxSquadSize = 8;
    RoundTime = 600.0f; // 10 minutes

    // Initialize faction scores
    m_FactionScores.Add(InvasionCycleConstants::EFactionType::Human, 0);
    m_FactionScores.Add(InvasionCycleConstants::EFactionType::Vyrex, 0);
}

bool GameMode::Initialize()
{
    m_IsGameActive = false;
    m_RemainingRoundTime = RoundTime;
    m_FactionScores.clear();
    m_Players.clear();
    m_ActiveCharacters.clear();
    return true;
}

void GameMode::Update(float deltaTime)
{
    if (!m_IsGameActive) return;

    m_RemainingRoundTime -= deltaTime;
    if (m_RemainingRoundTime <= 0.0f) {
        EndGame();
        return;
    }

    // Update all active characters
    for (auto& character : m_ActiveCharacters) {
        if (character) {
            character->Update(deltaTime);
        }
    }
}

void GameMode::Render()
{
    // Render all active characters
    for (auto& character : m_ActiveCharacters) {
        if (character) {
            character->Render();
        }
    }
}

void GameMode::Shutdown()
{
    m_Players.clear();
    m_ActiveCharacters.clear();
    m_FactionScores.clear();
}

void GameMode::StartGame()
{
    m_IsGameActive = true;
    m_RemainingRoundTime = RoundTime;
    m_FactionScores.clear();
}

void GameMode::EndGame()
{
    m_IsGameActive = false;
    // Calculate final scores and determine winner
}

void GameMode::PauseGame()
{
    m_IsGameActive = false;
}

void GameMode::ResumeGame()
{
    m_IsGameActive = true;
}

void GameMode::AddPlayer(std::shared_ptr<ICPlayerController> player)
{
    if (player) {
        m_Players.push_back(player);
    }
}

void GameMode::RemovePlayer(std::shared_ptr<ICPlayerController> player)
{
    auto it = std::find(m_Players.begin(), m_Players.end(), player);
    if (it != m_Players.end()) {
        m_Players.erase(it);
    }
}

void GameMode::SetPlayerFaction(std::shared_ptr<ICPlayerController> player, EFactionType faction)
{
    if (player) {
        player->SetFaction(faction);
    }
}

EFactionType GameMode::GetPlayerFaction(std::shared_ptr<ICPlayerController> player) const
{
    return player ? player->GetFaction() : EFactionType::None;
}

int GameMode::GetFactionScore(EFactionType faction) const
{
    auto it = m_FactionScores.find(faction);
    return it != m_FactionScores.end() ? it->second : 0;
}

void GameMode::AddFactionScore(EFactionType faction, int points)
{
    m_FactionScores[faction] += points;
}

void GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (m_IsGameActive)
    {
        Update(DeltaTime);
        ProcessRespawns(DeltaTime);
    }
}

void GameMode::SpawnPlayer(APlayerController* PlayerController, InvasionCycleConstants::EFactionType Faction)
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

void GameMode::RespawnPlayer(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        RespawnTimers.Add(PlayerController, RespawnDelay);
    }
}

void GameMode::CreateSquad(APlayerController* Leader, const FString& SquadName)
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

void GameMode::AddToSquad(APlayerController* Member, APlayerController* SquadLeader)
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

void GameMode::InitializeGame()
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

void GameMode::ProcessRespawns(float DeltaTime)
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

ATargetPoint* GameMode::GetSpawnPoint(InvasionCycleConstants::EFactionType Faction)
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

void GameMode::UpdateScores()
{
    // Reset scores
    m_FactionScores[InvasionCycleConstants::EFactionType::Human] = 0;
    m_FactionScores[InvasionCycleConstants::EFactionType::Vyrex] = 0;

    // Count living players for each faction
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = It->Get();
        if (PlayerController && PlayerController->GetPawn())
        {
            AICBaseCharacter* Character = Cast<AICBaseCharacter>(PlayerController->GetPawn());
            if (Character && !Character->IsDead())
            {
                m_FactionScores[Character->FactionType]++;
            }
        }
    }
}

} // namespace InvasionCycle 