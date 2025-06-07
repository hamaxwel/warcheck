#include "Tactical/ICTacticalCommandManager.h"
#include "Characters/ICBaseCharacter.h"

AICTacticalCommandManager::AICTacticalCommandManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AICTacticalCommandManager::BeginPlay()
{
    Super::BeginPlay();
}

void AICTacticalCommandManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ProcessCommandQueue();
}

void AICTacticalCommandManager::CreateSquad(AActor* Leader)
{
    if (Leader && !Squads.Contains(Leader))
    {
        InitializeSquad(Leader);
    }
}

void AICTacticalCommandManager::AddToSquad(AActor* Member, AActor* SquadLeader)
{
    if (Member && SquadLeader && Squads.Contains(SquadLeader) && CanJoinSquad(Member, SquadLeader))
    {
        FSquadData& Squad = Squads[SquadLeader];
        if (!Squad.Members.Contains(Member))
        {
            Squad.Members.Add(Member);
            
            // Notify the member that they've joined the squad
            if (ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(Member))
            {
                CommandInterface->Execute_JoinSquad(Member, SquadLeader);
            }
        }
    }
}

void AICTacticalCommandManager::RemoveFromSquad(AActor* Member)
{
    if (!Member)
        return;

    // Find the squad containing this member
    for (auto& SquadPair : Squads)
    {
        FSquadData& Squad = SquadPair.Value;
        if (Squad.Members.Contains(Member))
        {
            Squad.Members.Remove(Member);
            
            // Notify the member that they've left the squad
            if (ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(Member))
            {
                CommandInterface->Execute_LeaveSquad(Member);
            }

            // If this was the last member, clean up the squad
            if (Squad.Members.Num() == 0)
            {
                CleanupSquad(SquadPair.Key);
            }
            break;
        }
    }
}

void AICTacticalCommandManager::IssueCommand(const FCommandData& Command, AActor* Target)
{
    if (Target && ValidateCommand(Command, Target))
    {
        if (ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(Target))
        {
            CommandInterface->Execute_ExecuteCommand(Target, Command);
        }
    }
}

void AICTacticalCommandManager::IssueSquadCommand(const FCommandData& Command, AActor* SquadLeader)
{
    if (SquadLeader && Squads.Contains(SquadLeader))
    {
        FSquadData& Squad = Squads[SquadLeader];
        
        // Add command to squad's command queue
        Squad.CommandQueue.Add(Command);
        
        // Issue command to all squad members
        for (AActor* Member : Squad.Members)
        {
            if (Member && ValidateCommand(Command, Member))
            {
                IssueCommand(Command, Member);
            }
        }
    }
}

bool AICTacticalCommandManager::ValidateCommand(const FCommandData& Command, AActor* Target) const
{
    if (!Target)
        return false;

    if (ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(Target))
    {
        return CommandInterface->Execute_CanExecuteCommand(Target, Command);
    }
    return false;
}

void AICTacticalCommandManager::ProcessCommandQueue()
{
    // Process each squad's command queue
    for (auto& SquadPair : Squads)
    {
        FSquadData& Squad = SquadPair.Value;
        
        // Sort commands by priority and urgency
        Squad.CommandQueue.Sort([](const FCommandData& A, const FCommandData& B) {
            if (A.bIsUrgent != B.bIsUrgent)
                return A.bIsUrgent > B.bIsUrgent;
            return A.Priority > B.Priority;
        });

        // Process commands
        for (int32 i = Squad.CommandQueue.Num() - 1; i >= 0; --i)
        {
            const FCommandData& Command = Squad.CommandQueue[i];
            bool bAllComplete = true;
            bool bAnyFailed = false;

            // Check if all squad members have completed or failed the command
            for (AActor* Member : Squad.Members)
            {
                if (Member && ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(Member))
                {
                    if (!CommandInterface->Execute_IsCommandComplete(Member))
                    {
                        bAllComplete = false;
                        
                        // Check if command has failed (e.g., target is invalid)
                        if (Command.Type == ITacticalCommandInterface::ECommandType::Attack && 
                            (!Command.TargetActor || !IsValid(Command.TargetActor)))
                        {
                            bAnyFailed = true;
                            break;
                        }
                    }
                }
            }

            // Remove command if complete or failed
            if (bAllComplete || bAnyFailed)
            {
                Squad.CommandQueue.RemoveAt(i);
                
                // If command failed and was urgent, notify squad members
                if (bAnyFailed && Command.bIsUrgent)
                {
                    for (AActor* Member : Squad.Members)
                    {
                        if (Member && ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(Member))
                        {
                            CommandInterface->Execute_CancelCommand(Member);
                        }
                    }
                }
            }
        }

        // Update squad status
        UpdateSquadStatus(SquadPair.Key);
    }
}

TArray<AActor*> AICTacticalCommandManager::GetSquadMembers(AActor* SquadLeader) const
{
    if (SquadLeader && Squads.Contains(SquadLeader))
    {
        return Squads[SquadLeader].Members;
    }
    return TArray<AActor*>();
}

bool AICTacticalCommandManager::IsInSquad(AActor* Member) const
{
    if (!Member)
        return false;

    for (const auto& SquadPair : Squads)
    {
        if (SquadPair.Value.Members.Contains(Member))
        {
            return true;
        }
    }
    return false;
}

void AICTacticalCommandManager::InitializeSquad(AActor* Leader)
{
    if (Leader)
    {
        FSquadData NewSquad;
        NewSquad.Leader = Leader;
        NewSquad.Members.Add(Leader);
        Squads.Add(Leader, NewSquad);
    }
}

void AICTacticalCommandManager::CleanupSquad(AActor* SquadLeader)
{
    if (SquadLeader)
    {
        Squads.Remove(SquadLeader);
    }
}

void AICTacticalCommandManager::UpdateSquadStatus(AActor* SquadLeader)
{
    if (SquadLeader && Squads.Contains(SquadLeader))
    {
        FSquadData& Squad = Squads[SquadLeader];
        
        // Remove any invalid members
        Squad.Members.RemoveAll([](AActor* Member) {
            return !IsValid(Member);
        });

        // If no valid members remain, clean up the squad
        if (Squad.Members.Num() == 0)
        {
            CleanupSquad(SquadLeader);
        }
    }
}

bool AICTacticalCommandManager::CanJoinSquad(AActor* Member, AActor* SquadLeader) const
{
    if (!Member || !SquadLeader)
        return false;

    // Check if member is already in a squad
    if (IsInSquad(Member))
        return false;

    // Check if member and leader are of the same faction
    AICBaseCharacter* MemberChar = Cast<AICBaseCharacter>(Member);
    AICBaseCharacter* LeaderChar = Cast<AICBaseCharacter>(SquadLeader);
    
    if (MemberChar && LeaderChar)
    {
        return MemberChar->FactionType == LeaderChar->FactionType;
    }

    return false;
}

void AICTacticalCommandManager::SetSquadFormation(AActor* SquadLeader, ESquadFormation NewFormation)
{
    if (SquadLeader && Squads.Contains(SquadLeader))
    {
        FSquadData& Squad = Squads[SquadLeader];
        Squad.Formation = NewFormation;
        UpdateFormationPositions(SquadLeader);
    }
}

void AICTacticalCommandManager::AssignSquadRole(AActor* Member, ESquadRole NewRole)
{
    if (!Member)
        return;

    // Find the squad containing this member
    for (auto& SquadPair : Squads)
    {
        FSquadData& Squad = SquadPair.Value;
        for (FSquadMemberData& MemberData : Squad.Members)
        {
            if (MemberData.Member == Member)
            {
                MemberData.Role = NewRole;
                UpdateFormationPositions(SquadPair.Key);
                break;
            }
        }
    }
}

ESquadFormation AICTacticalCommandManager::GetSquadFormation(AActor* SquadLeader) const
{
    if (SquadLeader && Squads.Contains(SquadLeader))
    {
        return Squads[SquadLeader].Formation;
    }
    return ESquadFormation::Line;
}

ESquadRole AICTacticalCommandManager::GetSquadRole(AActor* Member) const
{
    if (!Member)
        return ESquadRole::Support;

    for (const auto& SquadPair : Squads)
    {
        for (const FSquadMemberData& MemberData : SquadPair.Value.Members)
        {
            if (MemberData.Member == Member)
            {
                return MemberData.Role;
            }
        }
    }
    return ESquadRole::Support;
}

void AICTacticalCommandManager::UpdateFormationPositions(AActor* SquadLeader)
{
    if (!SquadLeader || !Squads.Contains(SquadLeader))
        return;

    FSquadData& Squad = Squads[SquadLeader];
    
    // Update formation offsets for all members
    for (FSquadMemberData& MemberData : Squad.Members)
    {
        MemberData.FormationOffset = CalculateFormationOffset(Squad, MemberData);
    }
}

FVector AICTacticalCommandManager::CalculateFormationOffset(const FSquadData& Squad, const FSquadMemberData& Member) const
{
    if (!Squad.Leader)
        return FVector::ZeroVector;

    const float Spacing = Squad.FormationSpacing;
    const FVector LeaderForward = Squad.Leader->GetActorForwardVector();
    const FVector LeaderRight = Squad.Leader->GetActorRightVector();

    switch (Squad.Formation)
    {
        case ESquadFormation::Line:
        {
            // Line formation: Members spread out horizontally
            int32 MemberIndex = Squad.Members.IndexOfByPredicate([&](const FSquadMemberData& Data) {
                return Data.Member == Member.Member;
            });
            if (MemberIndex != INDEX_NONE)
            {
                float Offset = (MemberIndex - (Squad.Members.Num() - 1) / 2.0f) * Spacing;
                return LeaderRight * Offset;
            }
            break;
        }
        case ESquadFormation::Column:
        {
            // Column formation: Members follow in a line
            int32 MemberIndex = Squad.Members.IndexOfByPredicate([&](const FSquadMemberData& Data) {
                return Data.Member == Member.Member;
            });
            if (MemberIndex != INDEX_NONE)
            {
                return -LeaderForward * (MemberIndex * Spacing);
            }
            break;
        }
        case ESquadFormation::Wedge:
        {
            // Wedge formation: V-shaped formation
            switch (Member.Role)
            {
                case ESquadRole::Leader:
                    return FVector::ZeroVector;
                case ESquadRole::Point:
                    return LeaderForward * Spacing;
                case ESquadRole::Flank:
                    return (LeaderForward * Spacing * 0.5f) + (LeaderRight * Spacing);
                case ESquadRole::Rear:
                    return -LeaderForward * Spacing;
                case ESquadRole::Support:
                    return (LeaderForward * Spacing * 0.5f) - (LeaderRight * Spacing);
                default:
                    break;
            }
            break;
        }
        case ESquadFormation::Echelon:
        {
            // Echelon formation: Diagonal line
            int32 MemberIndex = Squad.Members.IndexOfByPredicate([&](const FSquadMemberData& Data) {
                return Data.Member == Member.Member;
            });
            if (MemberIndex != INDEX_NONE)
            {
                return (LeaderForward * MemberIndex * Spacing * 0.5f) + 
                       (LeaderRight * MemberIndex * Spacing);
            }
            break;
        }
        case ESquadFormation::Diamond:
        {
            // Diamond formation: Diamond-shaped formation
            switch (Member.Role)
            {
                case ESquadRole::Leader:
                    return FVector::ZeroVector;
                case ESquadRole::Point:
                    return LeaderForward * Spacing;
                case ESquadRole::Flank:
                    return LeaderRight * Spacing;
                case ESquadRole::Rear:
                    return -LeaderForward * Spacing;
                case ESquadRole::Support:
                    return -LeaderRight * Spacing;
                default:
                    break;
            }
            break;
        }
    }

    return FVector::ZeroVector;
} 