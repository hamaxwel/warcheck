#include "Core/ICPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Characters/ICBaseCharacter.h"
#include "Tactical/ICTacticalCommandManager.h"
#include "Tactical/ICTacticalCommandInterface.h"
#include <glm/gtc/matrix_transform.hpp>

namespace InvasionCycle {

ICPlayerController::ICPlayerController()
    : m_Faction(EFactionType::None)
    , m_MovementInput(0.0f)
    , m_TurnInput(0.0f)
    , m_LookUpInput(0.0f)
    , m_IsJumping(false)
    , m_IsFiring(false)
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

bool ICPlayerController::Initialize()
{
    m_PossessedCharacter.reset();
    m_Faction = EFactionType::None;
    m_MovementInput = glm::vec3(0.0f);
    m_TurnInput = 0.0f;
    m_LookUpInput = 0.0f;
    m_IsJumping = false;
    m_IsFiring = false;
    return true;
}

void ICPlayerController::Update(float deltaTime)
{
    if (m_PossessedCharacter) {
        // Apply movement input
        m_PossessedCharacter->Move(m_MovementInput);
        
        // Apply rotation input
        m_PossessedCharacter->Turn(m_TurnInput);
        m_PossessedCharacter->LookUp(m_LookUpInput);

        // Handle jumping
        if (m_IsJumping) {
            m_PossessedCharacter->Jump();
            m_IsJumping = false;
        }

        // Handle firing
        if (m_IsFiring) {
            m_PossessedCharacter->Fire();
            m_IsFiring = false;
        }

        // Update character
        m_PossessedCharacter->Update(deltaTime);
    }
}

void ICPlayerController::Render()
{
    if (m_PossessedCharacter) {
        m_PossessedCharacter->Render();
    }
}

void ICPlayerController::Shutdown()
{
    UnpossessCharacter();
}

void ICPlayerController::PossessCharacter(std::shared_ptr<ICBaseCharacter> character)
{
    if (m_PossessedCharacter) {
        UnpossessCharacter();
    }
    m_PossessedCharacter = character;
    if (m_PossessedCharacter) {
        m_PossessedCharacter->SetController(this);
    }
}

void ICPlayerController::UnpossessCharacter()
{
    if (m_PossessedCharacter) {
        m_PossessedCharacter->SetController(nullptr);
        m_PossessedCharacter.reset();
    }
}

void ICPlayerController::HandleInput()
{
    // Reset input values
    m_MovementInput = glm::vec3(0.0f);
    m_TurnInput = 0.0f;
    m_LookUpInput = 0.0f;

    // TODO: Implement actual input handling using GLFW
    // This is where we would check for key presses and update the input values
}

void ICPlayerController::MoveForward(float value)
{
    m_MovementInput.z = value;
}

void ICPlayerController::MoveRight(float value)
{
    m_MovementInput.x = value;
}

void ICPlayerController::Turn(float value)
{
    m_TurnInput = value;
}

void ICPlayerController::LookUp(float value)
{
    m_LookUpInput = value;
}

void ICPlayerController::Jump()
{
    m_IsJumping = true;
}

void ICPlayerController::Fire()
{
    m_IsFiring = true;
}

void ICPlayerController::BeginPlay()
{
    Super::BeginPlay();
    InitializeCamera();
}

void ICPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateCamera(DeltaTime);

    if (bIsCommanding)
    {
        ProcessCommandInput();
    }
}

void ICPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // View switching
    InputComponent->BindAction("TacticalView", IE_Pressed, this, &ICPlayerController::OnTacticalViewPressed);
    InputComponent->BindAction("TacticalView", IE_Released, this, &ICPlayerController::OnTacticalViewReleased);

    // Command input
    InputComponent->BindAction("Command", IE_Pressed, this, &ICPlayerController::OnCommandPressed);
    InputComponent->BindAction("Command", IE_Released, this, &ICPlayerController::OnCommandReleased);

    // Squad management
    InputComponent->BindAction("CreateSquad", IE_Pressed, this, &ICPlayerController::OnSquadCreatePressed);
    InputComponent->BindAction("JoinSquad", IE_Pressed, this, &ICPlayerController::OnSquadJoinPressed);
}

void ICPlayerController::SwitchToTacticalView()
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

void ICPlayerController::SwitchToFirstPersonView()
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

void ICPlayerController::IssueMoveCommand(const FVector& TargetLocation)
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

void ICPlayerController::IssueAttackCommand(AActor* Target)
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

void ICPlayerController::IssueDefendCommand(const FVector& Location)
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

void ICPlayerController::IssueSpecialCommand(const FVector& Location)
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

void ICPlayerController::CreateSquad()
{
    if (AICGameMode* GameMode = Cast<AICGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->CreateSquad(this, TEXT("Squad"));
    }
}

void ICPlayerController::JoinSquad(APlayerController* SquadLeader)
{
    if (AICGameMode* GameMode = Cast<AICGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->AddToSquad(this, SquadLeader);
    }
}

void ICPlayerController::OnTacticalViewPressed()
{
    SwitchToTacticalView();
}

void ICPlayerController::OnTacticalViewReleased()
{
    SwitchToFirstPersonView();
}

void ICPlayerController::OnCommandPressed()
{
    bIsCommanding = true;
}

void ICPlayerController::OnCommandReleased()
{
    bIsCommanding = false;
}

void ICPlayerController::OnSquadCreatePressed()
{
    CreateSquad();
}

void ICPlayerController::OnSquadJoinPressed()
{
    // TODO: Implement squad join UI
}

void ICPlayerController::InitializeCamera()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        // Attach camera to pawn
        CameraBoom->AttachToComponent(ControlledPawn->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }
}

void ICPlayerController::UpdateCamera(float DeltaTime)
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

void ICPlayerController::ProcessCommandInput()
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

FVector ICPlayerController::GetMouseWorldPosition() const
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

} // namespace InvasionCycle 