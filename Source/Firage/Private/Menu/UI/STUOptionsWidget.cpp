// Firage game:


#include "Menu/UI/STUOptionsWidget.h"

#include "STUMenuGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void USTUOptionsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &USTUOptionsWidget::OnMainMenu);
    }
}

void USTUOptionsWidget::OnMainMenu()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    const auto GameMode = Cast<ASTUMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->MainMenu();
}