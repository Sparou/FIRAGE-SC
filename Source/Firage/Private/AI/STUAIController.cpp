// Firage game:


#include "AI/STUAIController.h"
#include "STUAICharacter.h"
#include "STURespawnComponent.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIController, All, All)

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);

    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
    
    bWantsPlayerState = true;
}

FVector ASTUAIController::GetFocalPointOnActor(const AActor* Actor) const
{
    return Actor != nullptr ? Actor->GetActorLocation() + EnemyHeight : FAISystem::InvalidLocation;
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter)
    {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
    if(!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
