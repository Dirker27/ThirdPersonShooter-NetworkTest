// (C) ToasterCat Studios 2025

#include "Game/TPSGameState.h"

ATPSGameState::ATPSGameState()
{
    WorldState = CreateDefaultSubobject<UTPSWorldManager>(TEXT("WorldState"));
    TeamState = CreateDefaultSubobject<UTPSTeamManager>(TEXT("TeamState"));
}

UTPSTeam* ATPSGameState::GetTeam(ETPSTeamID teamId)
{
    return TeamState->GetTeam(teamId);
}

UTPSCommandStructure* ATPSGameState::GetUnit(FTPSUnitID id)
{
    if (auto team = TeamState->GetTeam(id.TeamID))
    {
        team->RootCollection;
    }
    return nullptr;
}

UTPSCharacterInstance* ATPSGameState::GetUnitLeader(FTPSUnitID id)
{
    if (auto unit = GetUnit(id))
    {
        return unit->GetLeader();
    }
    return nullptr;
}
