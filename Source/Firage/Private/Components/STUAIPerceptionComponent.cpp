// Firage game:

#include "Components/STUAIPerceptionComponent.h"
#include "STUAIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

AActor* USTUAIPerceptionComponent::GetVisibleEnemy() const
{
    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    TArray<FPerceptionData> PercieveActorsData;
    AActor* BestPawn = nullptr;

    GetCurrentPerceivedActorsData(UAISense_Sight::StaticClass(), PercieveActorsData);
    BestPawn = FindClosestPawn(Controller, Pawn, PercieveActorsData);

    return BestPawn;
}

FVector USTUAIPerceptionComponent::GetInterestLocation() const
{
    TArray<FPerceptionData> PercieveActorsData;
    FVector BestLocation = FVector(NAN, NAN, NAN);

    const auto Controller = Cast<AAIController>(GetOwner());
    if (Controller)
    {
        const auto Pawn = Controller->GetPawn();
        if (Pawn)
        {
            // Damage
            GetCurrentPerceivedActorsData(UAISense_Damage::StaticClass(), PercieveActorsData);
            BestLocation = FindClosestLocation(Pawn, PercieveActorsData);

            // Hearing
            if (BestLocation.ContainsNaN())
            {
                GetCurrentPerceivedActorsData(UAISense_Hearing::StaticClass(), PercieveActorsData);
                BestLocation = FindClosestLocation(Pawn, PercieveActorsData);
            }
        }
    }

    return BestLocation;
}

void USTUAIPerceptionComponent::GetCurrentPerceivedActorsData(
    TSubclassOf<UAISense> SenseToUse, TArray<FPerceptionData>& OutActorsData) const
{
    const FAISenseID SenseID = UAISense::GetSenseID(SenseToUse);
    FPerceptionData CurrentActorData;
     const FActorPerceptionContainer& PerceiveData = GetPerceptualData();
    OutActorsData.Reserve(PerceiveData.Num());
     for (FActorPerceptionContainer::TConstIterator DataIt = GetPerceptualDataConstIterator(); DataIt; ++DataIt)
     {
         const bool bCurrentlyPerceived =
             (SenseToUse == nullptr) ? DataIt->Value.HasAnyCurrentStimulus() : DataIt->Value.IsSenseActive(SenseID);
         if (bCurrentlyPerceived)
         {
             if (AActor* Actor = DataIt->Value.Target.Get())
             {
                CurrentActorData.Instigator = Actor;
                 const auto loc = Actor->GetActorLocation();
                CurrentActorData.SourceLocation = DataIt->Value.GetLastStimulusLocation();
                 //if (SenseToUse == UAISense_Hearing::StaticClass())
                 //{
                 //    UE_LOG(LogTemp, Display, TEXT("Actor: %s :: Location: %f %f %f || Source: %f %f %f"), *Actor->GetName(), loc.X, loc.Y, loc.Z,
                 //        CurrentActorData.SourceLocation.X, CurrentActorData.SourceLocation.Y, CurrentActorData.SourceLocation.Z);
                 //}
                OutActorsData.Add(CurrentActorData);
             }
         }
     }
}

AActor* USTUAIPerceptionComponent::FindClosestPawn(
    AAIController* Controller, APawn* Pawn, TArray<FPerceptionData>& PercieveActorsData) const
{
     float BestDistance = MAX_FLT;
     AActor* BestPawn = nullptr;

     for (const auto PercieveActorData : PercieveActorsData)
     {
         const auto PercieveActor = PercieveActorData.Instigator;
         const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);

         const auto PerceivePawn = Cast<APawn>(PercieveActor);
         const auto AreEnemies = PerceivePawn && STUUtils::AreEnemies(Controller, PerceivePawn->Controller);

         if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
         {
             const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
             if (CurrentDistance < BestDistance)
             {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
             }
         }
     }

     return BestPawn;
}

FVector USTUAIPerceptionComponent::FindClosestLocation(APawn* Pawn, TArray<FPerceptionData>& PercieveActorsData) const
{
     float BestDistance = MAX_FLT;
     FVector BestLocation = FVector(NAN, NAN, NAN);

     for (const auto PercieveActorData : PercieveActorsData)
     {
         const auto PercieveLocation = PercieveActorData.SourceLocation;
         const auto PerceivePawn = Cast<APawn>(PercieveActorData.Instigator);
         const auto AreEnemies = PerceivePawn && STUUtils::AreEnemies(Pawn->GetController(), PerceivePawn->Controller);
         UClass* ActorClass = PercieveActorData.Instigator->GetClass();

         if (ReactSoundsFrom.Contains(ActorClass) && AreEnemies)
         {
             const auto CurrentDistance = (PercieveLocation - Pawn->GetActorLocation()).Size();
             if (CurrentDistance < BestDistance)
             {
                BestDistance = CurrentDistance;
                BestLocation = PercieveLocation;
             }
         }
     }

     return BestLocation;
}