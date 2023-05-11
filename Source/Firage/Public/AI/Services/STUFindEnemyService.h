//Firage game:

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFindEnemyService.generated.h"

UCLASS()
class Firage_API USTUFindEnemyService : public UBTService
{
    GENERATED_BODY()
public:
    USTUFindEnemyService();
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
    FBlackboardKeySelector InterestLocationKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
