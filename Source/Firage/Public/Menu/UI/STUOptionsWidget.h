// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUOptionsWidget.generated.h"

class USTUGameInstance;
class USoundCue;
class UButton;


UCLASS()
class Firage_API USTUOptionsWidget : public USTUBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnMainMenu();
};
