// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STUAIPerceptionComponent.generated.h"

USTRUCT()
struct FPerceptionData
{
    GENERATED_BODY()

    AActor* Instigator = nullptr;
    FVector SourceLocation;
};

UCLASS()
class Firage_API USTUAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()
public:
    AActor* GetVisibleEnemy() const;
    FVector GetInterestLocation() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TSet<UClass*> ReactSoundsFrom;

private:
    AActor* FindClosestPawn(AAIController* Controller, APawn* Pawn, TArray<FPerceptionData>& PercieveActorsData) const;
    FVector FindClosestLocation(APawn* Pawn, TArray<FPerceptionData>& PercieveActorsData) const;
    void GetCurrentPerceivedActorsData(TSubclassOf<UAISense> SenseToUse, TArray<FPerceptionData>& OutActorsData) const;
};
