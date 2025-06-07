#include "UI/ICTacticalHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"

AICTacticalHUD::AICTacticalHUD()
{
    CommandFeedbackDuration = 2.0f;
    MoveCommandColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.5f);
    AttackCommandColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.5f);
    DefendCommandColor = FLinearColor(0.0f, 0.0f, 1.0f, 0.5f);
    SpecialCommandColor = FLinearColor(1.0f, 1.0f, 0.0f, 0.5f);
}

void AICTacticalHUD::BeginPlay()
{
    Super::BeginPlay();

    if (TacticalWidgetClass)
    {
        TacticalWidget = CreateWidget<UUserWidget>(GetWorld(), TacticalWidgetClass);
        if (TacticalWidget)
        {
            TacticalWidget->AddToViewport();
        }
    }
}

void AICTacticalHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateCommandFeedback(DeltaTime);
}

void AICTacticalHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCommandFeedback();
    DrawSquadInfo();
}

void AICTacticalHUD::UpdateSquadInfo(const TArray<FSquadMemberInfo>& SquadMembers)
{
    CurrentSquadInfo = SquadMembers;
    
    // Update the tactical widget with new squad info
    if (TacticalWidget)
    {
        // TODO: Update widget with new squad info
        // This will be implemented in the Blueprint widget
    }
}

void AICTacticalHUD::ShowCommandFeedback(const FVector& Location, ECommandType CommandType)
{
    CommandFeedbackLocations.Add(Location);
    CommandFeedbackTypes.Add(CommandType);
    CommandFeedbackTimers.Add(CommandFeedbackDuration);
}

void AICTacticalHUD::ShowSquadJoinPrompt(APlayerController* SquadLeader)
{
    if (SquadJoinWidgetClass && !SquadJoinWidget)
    {
        SquadJoinWidget = CreateWidget<UUserWidget>(GetWorld(), SquadJoinWidgetClass);
        if (SquadJoinWidget)
        {
            SquadJoinWidget->AddToViewport();
        }
    }
}

void AICTacticalHUD::DrawCommandFeedback()
{
    for (int32 i = 0; i < CommandFeedbackLocations.Num(); i++)
    {
        if (CommandFeedbackTimers[i] > 0.0f)
        {
            FVector2D ScreenLocation;
            if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), CommandFeedbackLocations[i], ScreenLocation))
            {
                FLinearColor CommandColor;
                switch (CommandFeedbackTypes[i])
                {
                    case ECommandType::Move:
                        CommandColor = MoveCommandColor;
                        break;
                    case ECommandType::Attack:
                        CommandColor = AttackCommandColor;
                        break;
                    case ECommandType::Defend:
                        CommandColor = DefendCommandColor;
                        break;
                    case ECommandType::Special:
                        CommandColor = SpecialCommandColor;
                        break;
                    default:
                        CommandColor = FLinearColor::White;
                }

                // Draw command marker
                float Alpha = CommandFeedbackTimers[i] / CommandFeedbackDuration;
                CommandColor.A = Alpha;
                
                DrawCircle(ScreenLocation, 20.0f, 32, CommandColor);
                
                // Draw command type indicator
                FString CommandText;
                switch (CommandFeedbackTypes[i])
                {
                    case ECommandType::Move:
                        CommandText = TEXT("MOVE");
                        break;
                    case ECommandType::Attack:
                        CommandText = TEXT("ATTACK");
                        break;
                    case ECommandType::Defend:
                        CommandText = TEXT("DEFEND");
                        break;
                    case ECommandType::Special:
                        CommandText = TEXT("SPECIAL");
                        break;
                }
                
                DrawText(CommandText, FLinearColor::White, ScreenLocation.X, ScreenLocation.Y + 30.0f, nullptr, 1.2f);
            }
        }
    }
}

void AICTacticalHUD::UpdateCommandFeedback(float DeltaTime)
{
    for (int32 i = CommandFeedbackTimers.Num() - 1; i >= 0; i--)
    {
        CommandFeedbackTimers[i] -= DeltaTime;
        if (CommandFeedbackTimers[i] <= 0.0f)
        {
            CommandFeedbackLocations.RemoveAt(i);
            CommandFeedbackTypes.RemoveAt(i);
            CommandFeedbackTimers.RemoveAt(i);
        }
    }
}

void AICTacticalHUD::DrawSquadInfo()
{
    // Find tactical command manager
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
    
    if (FoundActors.Num() > 0)
    {
        AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
        if (CommandManager)
        {
            // Draw squad member status
            for (const auto& SquadPair : CommandManager->Squads)
            {
                const FSquadData& Squad = SquadPair.Value;
                
                // Draw formation type
                FString FormationText = TEXT("Formation: ");
                switch (Squad.Formation)
                {
                    case ESquadFormation::Line:
                        FormationText += TEXT("Line");
                        break;
                    case ESquadFormation::Column:
                        FormationText += TEXT("Column");
                        break;
                    case ESquadFormation::Wedge:
                        FormationText += TEXT("Wedge");
                        break;
                    case ESquadFormation::Echelon:
                        FormationText += TEXT("Echelon");
                        break;
                    case ESquadFormation::Diamond:
                        FormationText += TEXT("Diamond");
                        break;
                }
                
                DrawText(FormationText, FLinearColor::White, 10.0f, 10.0f, nullptr, 1.2f);
                
                // Draw squad member status
                float YOffset = 40.0f;
                for (const FSquadMemberData& MemberData : Squad.Members)
                {
                    if (MemberData.Member)
                    {
                        // Get member status
                        FString StatusText;
                        if (ITacticalCommandInterface* CommandInterface = Cast<ITacticalCommandInterface>(MemberData.Member))
                        {
                            StatusText = CommandInterface->Execute_GetStatusReport(MemberData.Member);
                        }
                        
                        // Get member role
                        FString RoleText;
                        switch (MemberData.Role)
                        {
                            case ESquadRole::Leader:
                                RoleText = TEXT("[Leader] ");
                                break;
                            case ESquadRole::Point:
                                RoleText = TEXT("[Point] ");
                                break;
                            case ESquadRole::Flank:
                                RoleText = TEXT("[Flank] ");
                                break;
                            case ESquadRole::Rear:
                                RoleText = TEXT("[Rear] ");
                                break;
                            case ESquadRole::Support:
                                RoleText = TEXT("[Support] ");
                                break;
                        }
                        
                        // Draw member info
                        FString MemberText = RoleText + MemberData.Member->GetName() + TEXT(": ") + StatusText;
                        DrawText(MemberText, FLinearColor::White, 10.0f, YOffset, nullptr, 1.0f);
                        YOffset += 20.0f;
                    }
                }
            }
        }
    }
} 