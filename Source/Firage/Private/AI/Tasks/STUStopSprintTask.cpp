// Firage game:

#include "AI/Tasks/STUStopSprintTask.h"
#include "AIController.h"
#include "AI/STUAICharacter.h"

USTUStopSprintTask::USTUStopSprintTask()
{
    NodeName = "Stop Sprint";
}

EBTNodeResult::Type USTUStopSprintTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto Pawn = Cast<ASTUAICharacter>(Controller->GetPawn());
    if (!Pawn) return EBTNodeResult::Failed;

    Pawn->StartRun(false);
    return EBTNodeResult::Succeeded;
}
