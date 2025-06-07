#include "Core/ICPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ICBaseCharacter.h"
#include "Tactical/ICTacticalCommandManager.h"
#include "Tactical/ICTacticalCommandInterface.h"

AICPlayerController::AICPlayerController()
{
    // Set default values
    TacticalViewHeight = 1000.0f;
    TacticalViewPitch = -60.0f;
    TacticalViewTransitionSpeed = 5.0f;
    bIsInTacticalView = false;
    bIsCommanding = false;

    // Create camera components
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 0.0f;
    CameraBoom->bUsePawnControlRotation = true;

    TacticalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TacticalCamera"));
    TacticalCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TacticalCamera->bUsePawnControlRotation = false;
}

void AICPlayerController::BeginPlay()
{
    Super::BeginPlay();
    InitializeCamera();
}

void AICPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateCamera(DeltaTime);

    if (bIsCommanding)
    {
        ProcessCommandInput();
    }
}

void AICPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // View switching
    InputComponent->BindAction("TacticalView", IE_Pressed, this, &AICPlayerController::OnTacticalViewPressed);
    InputComponent->BindAction("TacticalView", IE_Released, this, &AICPlayerController::OnTacticalViewReleased);

    // Command input
    InputComponent->BindAction("Command", IE_Pressed, this, &AICPlayerController::OnCommandPressed);
    InputComponent->BindAction("Command", IE_Released, this, &AICPlayerController::OnCommandReleased);

    // Squad management
    InputComponent->BindAction("CreateSquad", IE_Pressed, this, &AICPlayerController::OnSquadCreatePressed);
    InputComponent->BindAction("JoinSquad", IE_Pressed, this, &AICPlayerController::OnSquadJoinPressed);
}

void AICPlayerController::SwitchToTacticalView()
{
    if (!bIsInTacticalView)
    {
        bIsInTacticalView = true;
        
        // Set camera properties for tactical view
        CameraBoom->TargetArmLength = TacticalViewHeight;
        CameraBoom->SetRelativeRotation(FRotator(TacticalViewPitch, 0.0f, 0.0f));
        
        // Disable character movement
        if (APawn* ControlledPawn = GetPawn())
        {
            ControlledPawn->DisableInput(this);
        }
    }
}

void AICPlayerController::SwitchToFirstPersonView()
{
    if (bIsInTacticalView)
    {
        bIsInTacticalView = false;
        
        // Reset camera properties for first-person view
        CameraBoom->TargetArmLength = 0.0f;
        CameraBoom->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        
        // Enable character movement
        if (APawn* ControlledPawn = GetPawn())
        {
            ControlledPawn->EnableInput(this);
        }
    }
}

void AICPlayerController::IssueMoveCommand(const FVector& TargetLocation)
{
    if (bIsInTacticalView)
    {
        // Find tactical command manager
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
        
        if (FoundActors.Num() > 0)
        {
            AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
            if (CommandManager)
            {
                // Create move command
                ITacticalCommandInterface::FCommandData Command;
                Command.Type = ITacticalCommandInterface::ECommandType::Move;
                Command.TargetLocation = TargetLocation;
                Command.Priority = 1.0f;
                Command.bIsUrgent = false;

                // Issue command to squad
                CommandManager->IssueSquadCommand(Command, GetPawn());
            }
        }
    }
}

void AICPlayerController::IssueAttackCommand(AActor* Target)
{
    if (bIsInTacticalView && Target)
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
        
        if (FoundActors.Num() > 0)
        {
            AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
            if (CommandManager)
            {
                ITacticalCommandInterface::FCommandData Command;
                Command.Type = ITacticalCommandInterface::ECommandType::Attack;
                Command.TargetActor = Target;
                Command.Priority = 2.0f;
                Command.bIsUrgent = true;

                CommandManager->IssueSquadCommand(Command, GetPawn());
            }
        }
    }
}

void AICPlayerController::IssueDefendCommand(const FVector& Location)
{
    if (bIsInTacticalView)
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
        
        if (FoundActors.Num() > 0)
        {
            AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
            if (CommandManager)
            {
                ITacticalCommandInterface::FCommandData Command;
                Command.Type = ITacticalCommandInterface::ECommandType::Defend;
                Command.TargetLocation = Location;
                Command.Priority = 1.5f;
                Command.bIsUrgent = false;

                CommandManager->IssueSquadCommand(Command, GetPawn());
            }
        }
    }
}

void AICPlayerController::IssueSpecialCommand(const FVector& Location)
{
    if (bIsInTacticalView)
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AICTacticalCommandManager::StaticClass(), FoundActors);
        
        if (FoundActors.Num() > 0)
        {
            AICTacticalCommandManager* CommandManager = Cast<AICTacticalCommandManager>(FoundActors[0]);
            if (CommandManager)
            {
                ITacticalCommandInterface::FCommandData Command;
                Command.Type = ITacticalCommandInterface::ECommandType::Special;
                Command.TargetLocation = Location;
                Command.Priority = 3.0f;
                Command.bIsUrgent = true;

                CommandManager->IssueSquadCommand(Command, GetPawn());
            }
        }
    }
}

void AICPlayerController::CreateSquad()
{
    if (AICGameMode* GameMode = Cast<AICGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->CreateSquad(this, TEXT("Squad"));
    }
}

void AICPlayerController::JoinSquad(APlayerController* SquadLeader)
{
    if (AICGameMode* GameMode = Cast<AICGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->AddToSquad(this, SquadLeader);
    }
}

void AICPlayerController::OnTacticalViewPressed()
{
    SwitchToTacticalView();
}

void AICPlayerController::OnTacticalViewReleased()
{
    SwitchToFirstPersonView();
}

void AICPlayerController::OnCommandPressed()
{
    bIsCommanding = true;
}

void AICPlayerController::OnCommandReleased()
{
    bIsCommanding = false;
}

void AICPlayerController::OnSquadCreatePressed()
{
    CreateSquad();
}

void AICPlayerController::OnSquadJoinPressed()
{
    // TODO: Implement squad join UI
}

void AICPlayerController::InitializeCamera()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        // Attach camera to pawn
        CameraBoom->AttachToComponent(ControlledPawn->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }
}

void AICPlayerController::UpdateCamera(float DeltaTime)
{
    if (bIsInTacticalView)
    {
        // Smoothly transition to tactical view
        float CurrentHeight = CameraBoom->TargetArmLength;
        float TargetHeight = TacticalViewHeight;
        
        CameraBoom->TargetArmLength = FMath::FInterpTo(
            CurrentHeight,
            TargetHeight,
            DeltaTime,
            TacticalViewTransitionSpeed
        );

        // Update camera rotation
        FRotator CurrentRotation = CameraBoom->GetRelativeRotation();
        FRotator TargetRotation = FRotator(TacticalViewPitch, 0.0f, 0.0f);
        
        CameraBoom->SetRelativeRotation(FMath::RInterpTo(
            CurrentRotation,
            TargetRotation,
            DeltaTime,
            TacticalViewTransitionSpeed
        ));
    }
}

void AICPlayerController::ProcessCommandInput()
{
    if (bIsInTacticalView)
    {
        FVector MouseWorldPos = GetMouseWorldPosition();
        
        // Perform trace to find target
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(GetPawn());
        
        if (GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TacticalCamera->GetComponentLocation(),
            MouseWorldPos,
            ECC_Visibility,
            QueryParams
        ))
        {
            // Check if we hit an enemy
            if (AActor* HitActor = HitResult.GetActor())
            {
                if (AICBaseCharacter* HitCharacter = Cast<AICBaseCharacter>(HitActor))
                {
                    if (HitCharacter->FactionType != Cast<AICBaseCharacter>(GetPawn())->FactionType)
                    {
                        IssueAttackCommand(HitActor);
                        return;
                    }
                }
            }
            
            // If no enemy was hit, issue move command
            IssueMoveCommand(HitResult.Location);
        }
    }
}

FVector AICPlayerController::GetMouseWorldPosition() const
{
    float LocationX;
    float LocationY;
    GetMousePosition(LocationX, LocationY);

    FVector WorldLocation;
    FVector WorldDirection;
    DeprojectScreenPositionToWorld(LocationX, LocationY, WorldLocation, WorldDirection);

    // Calculate intersection with ground plane
    FVector CameraLocation = TacticalCamera->GetComponentLocation();
    FVector Intersection = FMath::LinePlaneIntersection(
        CameraLocation,
        CameraLocation + WorldDirection * 10000.0f,
        FVector(0.0f, 0.0f, 0.0f),
        FVector(0.0f, 0.0f, 1.0f)
    );

    return Intersection;
} 