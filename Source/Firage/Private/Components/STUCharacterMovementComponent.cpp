// Firage game:


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogMovementComponent, All, All)

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    
    if (Player && Player->IsRunning())
    {
        return MaxSpeed * RunModifier;
    }

    if (Player && Player->IsCrouching())
    {
        return MaxSpeed * CrouchModifier;
    }
    return MaxSpeed;
}
