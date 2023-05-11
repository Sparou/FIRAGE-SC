// Firage game:

#include "AI/STUAICharacter.h"
#include "STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() ||                              //
        !GetWorld()->GetFirstPlayerController() ||  //
        !GetWorld()->GetFirstPlayerController()->GetPawn())
        return;

    const auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    const auto PlayerLocation = PlayerPawn->GetActorLocation();
    const auto PlayerName = PlayerPawn->GetFName();

    FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
    TraceParams.bTraceComplex = true;

    FHitResult HitData(ForceInit);

    //DrawDebugLine(GetWorld(), PlayerLocation, GetActorLocation(), FColor::Red, false, 1.0f, (uint8)0U, 1.0f);
    GetWorld()->LineTraceSingleByChannel(HitData, GetActorLocation(), PlayerLocation, ECC_WorldDynamic, TraceParams);

    if (HitData.HitObjectHandle.GetFName() != PlayerName)
    {
        HealthWidgetComponent->SetVisibility(false, true);
    }
    else
    {
        const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
        HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();
    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
    Super::OnHealthChanged(Health, HealthDelta);

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;
    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);
}

bool ASTUAICharacter::IsMovingForward() const
{
    const auto LookVector = GetActorForwardVector();
    const auto DirectionVector = GetVelocity().GetSafeNormal();
    //UE_LOG(LogTemp, Display, TEXT("LOOK: %f; %f || MOVE: %f; %f"), LookVector.X, LookVector.Y, LookVector.Z,  //
    //    DirectionVector.X, DirectionVector.Y, DirectionVector.Z);
    if (FMath::Abs(LookVector.X - DirectionVector.X) > 0.9f || //
        FMath::Abs(LookVector.Y - DirectionVector.Y) > 0.9f)
    {
    //    UE_LOG(LogTemp, Error, TEXT("X: %f or Y: %f !!!"), FMath::Abs(LookVector.X) - FMath::Abs(DirectionVector.X),
    //        FMath::Abs(LookVector.Y) - FMath::Abs(DirectionVector.Y))
        return false;
    }
    return true;
}

bool ASTUAICharacter::IsRunning() const
{

    return WantToRun && IsMovingForward() && !GetVelocity().IsZero();
}