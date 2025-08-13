// (C) ToasterCat Studios 2025

#include "Game/TPSGameState.h"

#include "Net/UnrealNetwork.h"

ATPSGameState::ATPSGameState()
{
    bReplicates = true;

    WorldState = CreateDefaultSubobject<UTPSWorldManager>(TEXT("WorldState"));
    TeamState = CreateDefaultSubobject<UTPSTeamManager>(TEXT("TeamState"));
    CombatLog = CreateDefaultSubobject<UTPSCombatLog>(TEXT("CombatLog"));
}


void ATPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATPSGameState, TeamState);
    DOREPLIFETIME(ATPSGameState, WorldState);
}


int ATPSGameState::GetTeamScore(ETPSTeamID teamId)
{
    if (auto score = TeamScores.Find(teamId))
    {
        return *score;
    }
    return 0;
}