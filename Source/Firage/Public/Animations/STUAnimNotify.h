// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

/**
 * 
 */
UCLASS()
class Firage_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
    void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

    FOnNotifiedSignature OnNotified;
};
