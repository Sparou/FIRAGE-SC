// Firage game:


#include "Player/STUPlayerState.h"

#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All)

void ASTUPlayerState::LogInfo()
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("Team ID: %i | Kills: %i | Deaths: %i"), TeamID, KillsNum, DeathsNum);
}

