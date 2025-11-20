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
    DOREPLIFETIME(ThisClass, Armies);

    DOREPLIFETIME(ThisClass, ScoredPoints);
    DOREPLIFETIME(ThisClass, ActivePlayers);
}

bool UTPSTeamInstance::IsSupportedForNetworking() const
{
    return true;
}
