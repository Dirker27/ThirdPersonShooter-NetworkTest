// (C) ToasterCat Studios 2025

#include "Army/TPSArmy.h"

#include "Net/UnrealNetwork.h"

void UTPSArmyInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSArmyInstance, ArmyID);
    DOREPLIFETIME(UTPSArmyInstance, TeamID);
    DOREPLIFETIME(UTPSArmyInstance, Identity);
    DOREPLIFETIME(UTPSArmyInstance, RootUnit);
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

void UTPSArmyInstance::AssignTeam(const ETPSTeamID team)
{
    TeamID = team;
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::AssignRootUnit(UTPSCommandUnit* unit)
{
    RootUnit = unit;
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::AddUnit(UTPSCommandUnit* unit)
{
    Units.Add(unit);
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::AddMember(UTPSCharacterInstance* instance)
{
    Members.Add(instance);
    ArmyUpdate.Broadcast();
}

void UTPSArmyInstance::BindToPlayer(const FTPSPlayerID playerId)
{
    OwningPlayer = playerId;
}
