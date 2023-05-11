// Firage game:

#include "UI/STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPauseWidget, All, All)

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
        GoOptions->OnClicked.AddDynamic(this, &USTUPauseWidget::GoOptionsWidget);
    }
}

void USTUPauseWidget::OnClearPause()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}

void USTUPauseWidget::GoOptionsWidget() 
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->GoOptions();
}
