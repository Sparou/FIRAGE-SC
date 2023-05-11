// Firage game:


#include "Menu/UI/STUMenuWidget.h"

#include "STUMenuGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All)

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (ChooseGameButton)
    {
        ChooseGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::ChooseLevel);
    }

    if (OptionsGameButton)
    {
        OptionsGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::SetOptions);
    }

}

void USTUMenuWidget::ChooseLevel()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    
    const auto GameMode = Cast<ASTUMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->ChooseLevel();
}

void USTUMenuWidget::SetOptions()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    
    const auto GameMode = Cast<ASTUMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->SetOptions();
}