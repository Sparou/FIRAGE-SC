// Firage game:

#include "AI/Tasks/STUStartSprintTask.h"
#include "AIController.h"
#include "AI/STUAICharacter.h"

USTUStartSprintTask::USTUStartSprintTask()
{
    NodeName = "Start Sprint";
}

EBTNodeResult::Type USTUStartSprintTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto Pawn = Cast<ASTUAICharacter>(Controller->GetPawn());
    if (!Pawn) return EBTNodeResult::Failed;

    Pawn->StartRun(true);
    return EBTNodeResult::Succeeded;
}
