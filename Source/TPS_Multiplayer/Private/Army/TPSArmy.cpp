// (C) ToasterCat Studios 2025

#include "Army/TPSArmy.h"

#include "Team/TPSTeamInstance.h"

#include "Net/UnrealNetwork.h"

void UTPSArmyInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSArmyInstance, ArmyID);
    DOREPLIFETIME(UTPSArmyInstance, Identity);
    DOREPLIFETIME(UTPSArmyInstance, RootUnit);
    DOREPLIFETIME(UTPSArmyInstance, OwningPlayer);
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
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::BindToPlayer(ATPSPlayerState* player)
{
    OwningPlayer = player;
    ArmyUpdate.Broadcast();
}
