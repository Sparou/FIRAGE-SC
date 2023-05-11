// Firage game:


#include "AI/Decorators/STUNeedAmmoDecorator.h"

#include "AIController.h"
#include "STUAIWeaponComponent.h"
#include "STUUtils.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUAIWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->NeedAmmo(WeaponType);
}
