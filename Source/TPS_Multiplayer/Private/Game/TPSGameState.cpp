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

int ATPSGameState::GetTeamUnitCount(ETPSTeamID teamId)
{
    int count = 0;
    if (UTPSTeam* team = TeamState->GetTeam(teamId))
    {
        return TeamState->CountUnitMembers(team->RootCollection->UnitID);
    }
    return 0;
}



UTPSTeam* ATPSGameState::GetTeam(ETPSTeamID teamId)
{
    return TeamState->GetTeam(teamId);
}

UTPSCommandStructure* ATPSGameState::GetUnit(FTPSUnitID id)
{
    return TeamState->GetUnit(id);
}

UTPSCharacterInstance* ATPSGameState::GetUnitLeader(FTPSUnitID id)
{
    if (auto unit = GetUnit(id))
    {
        return unit->GetLeader();
    }
    return nullptr;
}



void ATPSGameState::CreateTeam(ETPSTeamID teamId)
{
    TeamState->CreateTeam(teamId);
}

void ATPSGameState::ConfigureTeam(ETPSTeamID teamId, FTPSTeamConfiguration configuration)
{
    TeamState->ConfigureTeam(teamId, configuration);
}

void ATPSGameState::PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster)
{
    TeamState->PopulateTeam(teamId, roster);
}

