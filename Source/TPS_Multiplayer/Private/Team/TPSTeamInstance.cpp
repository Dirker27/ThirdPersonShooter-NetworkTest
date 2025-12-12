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








UTPSCharacterInstance* UTPSTeamInstance::GetRandomActiveMember() const
{
    int startIndex = FMath::RandRange(0, Members.Num() - 1);

    if (Members[startIndex]->IsActive())
    {
        return Members[startIndex];
    }

    int i = startIndex + 1;
    if (i >= Members.Num()) { i = 0; }
    while (i != startIndex)
    {
        if (Members[i]->IsActive())
        {
            return Members[i];
        }

        i++;
        if (i >= Members.Num()) { i = 0; }
    }

    return nullptr;
}

UTPSCharacterInstance* UTPSTeamInstance::GetRandomPossessableMember() const
{
    if (Members.IsEmpty()) { return nullptr; }


    int startIndex = FMath::RandRange(0, Members.Num() - 1);

    if (Members[startIndex]->CanBePossessed())
    {
        return Members[startIndex];
    }

    int i = startIndex + 1;
    if (i >= Members.Num()) { i = 0; }
    while (i != startIndex)
    {
        if (Members[i]->CanBePossessed())
        {
            return Members[i];
        }

        i++;
        if (i >= Members.Num()) { i = 0; }
    }

    return nullptr;
}