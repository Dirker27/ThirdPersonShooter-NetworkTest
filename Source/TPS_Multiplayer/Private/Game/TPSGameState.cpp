// (C) ToasterCat Studios 2025

#include "Game/TPSGameState.h"

ATPSGameState::ATPSGameState()
{
    WorldState = CreateDefaultSubobject<UTPSWorldManager>(TEXT("WorldState"));
    TeamState = CreateDefaultSubobject<UTPSTeamManager>(TEXT("TeamState"));
    CombatLog = CreateDefaultSubobject<UTPSCombatLog>(TEXT("CombatLog"));
}


int ATPSGameState::GetTeamScore(ETPSTeamID teamId)
{
    if (auto score = TeamScores.Find(teamId))
    {
        return *score;
    }
    return 0;
}