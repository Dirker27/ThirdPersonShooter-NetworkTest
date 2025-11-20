// (C) ToasterCat Studios 2025

#include "Army/Unit/TPSCommandUnit.h"

#include "Net/UnrealNetwork.h"
#include "Team/TPSTeamInstance.h"

UTPSCommandUnit::UTPSCommandUnit()
{
}

void UTPSCommandUnit::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Identity);
	DOREPLIFETIME(ThisClass, AssignedTeam);
	DOREPLIFETIME(ThisClass, AssignedArmy);
	DOREPLIFETIME(ThisClass, Leader);
}

bool UTPSCommandUnit::IsSupportedForNetworking() const
{
	return true;
}


// TODO: Elevate to "TeamAssignable" Interface
ETPSTeamID UTPSCommandUnit::GetAssignedTeamID() const
{
	return IsValid(AssignedTeam.Get())
		? AssignedTeam->TeamID
		: ETPSTeamID::UNAFFILIATED;
}

void UTPSCommandUnit::AssignToArmy(UTPSArmyInstance* army)
{
	AssignedTeam = army->AssignedTeam;
	AssignedArmy = army;
}

void UTPSCommandUnit::SetLeader(UTPSCharacterInstance* member)
{
	Leader = member;
	UnitUpdate.Broadcast();
}

UTPSCharacterInstance* UTPSCommandUnit::GetLeader() const
{
	return Leader.Get();
}
