// Firage game:


#include "Menu/UI/STUMenuHUD.h"
#include "STUMenuGameModeBase.h"
#include "STUCoreTypes.h"
#include "STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(ESTUMenuState::MenuStart, CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass));
    GameWidgets.Add(ESTUMenuState::Options, CreateWidget<USTUBaseWidget>(GetWorld(), OptionsWidgetClass));
    GameWidgets.Add(ESTUMenuState::ChooseLevel, CreateWidget<USTUBaseWidget>(GetWorld(), ChooseLevelWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUMenuGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMenuStateChanged.AddUObject(this, &ASTUMenuHUD::OnMenuStateChanged);
        }
    }
}

void ASTUMenuHUD::OnMenuStateChanged(ESTUMenuState State)
{
    USTUBaseWidget* PrevWidget = CurrentWidget;
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        

        CurrentWidget->Show();
    }
}
