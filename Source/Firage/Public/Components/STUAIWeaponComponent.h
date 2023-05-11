// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "Components/STUWeaponComponent.h"
#include "STUAIWeaponComponent.generated.h"

UCLASS()
class Firage_API USTUAIWeaponComponent : public USTUWeaponComponent
{
    GENERATED_BODY()
public:
    void StartFire() override;
    void NextWeapon() override;
};
