// Firage

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;

UCLASS()
class Firage_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GoOptions;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    //TSubclassOf<UUserWidget> GoToPauseWidgetClass;
    
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClearPause();

    UFUNCTION()
    void GoOptionsWidget();
};
