// Firage game:


#include "UI/STUOptionsPauseWidget.h"

#include "STUGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void USTUOptionsPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ToPause)
    {
        ToPause->OnClicked.AddDynamic(this, &USTUOptionsPauseWidget::ReturnToPause);
    }
}

void USTUOptionsPauseWidget::ReturnToPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->ReturnPause();
}