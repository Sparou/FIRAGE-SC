// Firage game:


#include "AI/Services/STUFindEnemyService.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFindEnemyService, All, All)

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetVisibleEnemy());
            FVector InterestLocation = PerceptionComponent->GetInterestLocation();
            Blackboard->SetValueAsVector(InterestLocationKey.SelectedKeyName, InterestLocation);
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
