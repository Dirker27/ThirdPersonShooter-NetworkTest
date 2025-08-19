// (C) ToasterCat Studios 2025

#include "Team/TPSCommandStructure.h"

#include "Net/UnrealNetwork.h"

UTPSCommandStructure::UTPSCommandStructure()
{
}

void UTPSCommandStructure::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Leader);
}

bool UTPSCommandStructure::IsSupportedForNetworking() const
{
	return true;
}



void UTPSCommandStructure::SetLeader(UTPSCharacterInstance* member)
{
	Leader = member;
}

UTPSCharacterInstance* UTPSCommandStructure::GetLeader() const
{
	return Leader.Get();
}
