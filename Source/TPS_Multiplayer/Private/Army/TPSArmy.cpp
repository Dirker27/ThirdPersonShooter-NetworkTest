// (C) ToasterCat Studios 2025

#include "Army/TPSArmy.h"

#include "Team/TPSTeamInstance.h"

#include "Net/UnrealNetwork.h"

void UTPSArmyInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, ArmyID);
    DOREPLIFETIME(ThisClass, Identity);
    DOREPLIFETIME(ThisClass, RootUnit);
    DOREPLIFETIME(ThisClass, OwningPlayer);

    DOREPLIFETIME(ThisClass, Members);
    DOREPLIFETIME(ThisClass, ActiveMembers);
    DOREPLIFETIME(ThisClass, Units);
    DOREPLIFETIME(ThisClass, AssignedTeam);
}

bool UTPSArmyInstance::IsSupportedForNetworking() const
{
    return true;
}


void UTPSArmyInstance::Initialize(FTPSArmyID id, UTPSArmyIdentity* ident)
{
    if (IsInitialized) { return; }

    ArmyID = id;
    Identity = ident;

    IsInitialized = true;

    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::AssignToTeam(UTPSTeamInstance* team)
{
    AssignedTeam = team;
    ArmyUpdate.Broadcast();
}

// TODO: Elevate to "TeamAssignable" Interface
ETPSTeamID UTPSArmyInstance::GetAssignedTeamID() const
{
    return AssignedTeam.IsValid()
        ? AssignedTeam.Get()->TeamID
        : ETPSTeamID::UNAFFILIATED;
}

UTPSTeamInstance* UTPSArmyInstance::GetAssignedTeam() const
{
    return AssignedTeam.IsValid()
        ? AssignedTeam.Get()
        : nullptr;
}



void UTPSArmyInstance::AssignRootUnit(UTPSCommandUnit* unit)
{
    RootUnit = unit;
    RootUnit->AssignToArmy(this);
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::AddUnit(UTPSCommandUnit* unit)
{
    Units.Add(unit);
    unit->AssignToArmy(this);

    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::AddMember(UTPSCharacterInstance* instance)
{
    Members.Add(instance);
    instance->AssignToArmy(this);
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::BindToPlayer(ATPSPlayerState* player)
{
    OwningPlayer = player;
    ArmyUpdate.Broadcast();
}



UTPSCharacterInstance* UTPSArmyInstance::GetRandomActiveMember() const
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

UTPSCharacterInstance* UTPSArmyInstance::GetRandomPossessableMember() const
{
    if (Members.IsEmpty()) { return nullptr; }


    int startIndex = FMath::RandRange(0, Members.Num()-1);

    if (Members[startIndex]->CanBePossessed())
    {
        return Members[startIndex];
    }

    int i = startIndex+1;
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
