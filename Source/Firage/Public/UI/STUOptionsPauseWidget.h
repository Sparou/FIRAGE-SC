// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUOptionsPauseWidget.generated.h"

class UButton;

UCLASS()
class Firage_API USTUOptionsPauseWidget : public USTUBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ToPause;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void ReturnToPause();
};