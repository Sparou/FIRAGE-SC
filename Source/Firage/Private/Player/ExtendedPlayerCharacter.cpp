// Firage game:


#include "Player/ExtendedPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUWeaponComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogExtChar, All, All)

AExtendedPlayerCharacter::AExtendedPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    //bUseControllerRotationPitch = false;
    //bUseControllerRotationYaw = false;
    //bUseControllerRotationRoll = false;

    // Configure character movement
    //GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 700.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;       // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}


void AExtendedPlayerCharacter::BeginPlay()
{
    // Call the base class  
    Super::BeginPlay();

    //Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Input

void AExtendedPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        //Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        //Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExtendedPlayerCharacter::Move);

        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AExtendedPlayerCharacter::OnStartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AExtendedPlayerCharacter::OnStopCrouch);

        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::StopFire);

        //Sprinting
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AExtendedPlayerCharacter::OnStartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AExtendedPlayerCharacter::OnStopSprint);
        
        //Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExtendedPlayerCharacter::Look);

    }
}

/*
void AExtendedPlayerCharacter::MoveForward(const FInputActionValue& Value)
{
    IsMovingForward = Value.Get<float>() > 0.0f;
    if (Value.Get<float>() == 0) return;
    AddMovementInput(GetActorForwardVector(), Value.Get<float>());
}

void AExtendedPlayerCharacter::MoveRight(const FInputActionValue& Value)
{
    if (Value.Get<float>() == 0.0f) return;
    AddMovementInput(GetActorRightVector(),Value.Get<float>());
}
*/

void AExtendedPlayerCharacter::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement

        IsMovingForward = MovementVector.Y > 0.0f;
        
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}



void AExtendedPlayerCharacter::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(-1 * LookAxisVector.Y);
    }
}

void AExtendedPlayerCharacter::OnStartSprint()
{
    WantsToSprint = true;
}

void AExtendedPlayerCharacter::OnStopSprint()
{
    UE_LOG(LogExtChar, Display, TEXT("On Stop Sprint is called"))
    WantsToSprint = false;
}

void AExtendedPlayerCharacter::OnStartCrouch()
{
    WantsToCrouch = true;
}

void AExtendedPlayerCharacter::OnStopCrouch()
{
    WantsToCrouch = false;
}

bool AExtendedPlayerCharacter::IsRunning() const
{
    return WantsToSprint && IsMovingForward && !GetVelocity().IsZero();
}

bool AExtendedPlayerCharacter::IsCrouching() const
{
    return WantsToCrouch;
}

