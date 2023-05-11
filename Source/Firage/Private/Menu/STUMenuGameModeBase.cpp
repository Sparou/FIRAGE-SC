// Firage game:


#include "Menu/STUMenuGameModeBase.h"
#include "Menu/STUMenuPlayerController.h"
#include "STUCoreTypes.h"
#include "Menu/UI/STUMenuHUD.h"


ASTUMenuGameModeBase::ASTUMenuGameModeBase()
{
    PlayerControllerClass = ASTUMenuPlayerController::StaticClass();
    HUDClass = ASTUMenuHUD::StaticClass();
}

void ASTUMenuGameModeBase::StartPlay()
{
    Super::StartPlay();

    SetMenuState(ESTUMenuState::MenuStart);
}

void ASTUMenuGameModeBase::SetMenuState(ESTUMenuState State)
{
    if (MenuState == State) return;

    MenuState = State;
    OnMenuStateChanged.Broadcast(MenuState);
}