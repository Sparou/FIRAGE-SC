// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class USTUGameInstance;
class USoundCue;
class UButton;

UCLASS()
class Firage_API USTUMenuWidget : public USTUBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ChooseGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* OptionsGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* HideAnimation;

    
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION() 
    void ChooseLevel();

    UFUNCTION()
    void SetOptions();
};
