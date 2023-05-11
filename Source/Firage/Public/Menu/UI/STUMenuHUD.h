// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUMenuHUD.generated.h"

class USTUBaseWidget;

UCLASS()
class Firage_API ASTUMenuHUD : public AHUD
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> OptionsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> ChooseLevelWidgetClass;
    
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<ESTUMenuState, USTUBaseWidget*> GameWidgets;

    UPROPERTY()
    USTUBaseWidget* CurrentWidget = nullptr;

    void OnMenuStateChanged(ESTUMenuState State);
};
