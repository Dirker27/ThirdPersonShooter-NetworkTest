// (C) ToasterCat Studios 2025

#include "Team/TPSTeamInstance.h"

#include "World/Spawn/TPSSpawnPool.h"
#include "Net/UnrealNetwork.h"

UTPSTeamInstance::UTPSTeamInstance()
{
    SpawnPool = CreateDefaultSubobject<UTPSSpawnPool>(TEXT("SpawnPool"));
}


void UTPSTeamInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, TeamID);
    DOREPLIFETIME(ThisClass, ScoredPoints);

    DOREPLIFETIME(ThisClass, Members);
    DOREPLIFETIME(ThisClass, Armies);
    DOREPLIFETIME(ThisClass, Players);
}

void UTPSTeamInstance::AddMember(UTPSCharacterInstance* member)
{
    if (IsValid(member))
    {
        Members.Add(member);
        TeamUpdate.Broadcast();
    }
}

void UTPSTeamInstance::AddArmy(UTPSArmyInstance* army)
{
    if (IsValid(army))
    {
        Armies.Add(army);
        TeamUpdate.Broadcast();
    }
}

void UTPSTeamInstance::AddPlayer(ATPSPlayerState* player)
{
    if (IsValid(player))
    {
        Players.Add(player);
        TeamUpdate.Broadcast();
    }
}
