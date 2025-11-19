// (C) ToasterCat Studios 2025

#include "Army/Unit/TPSCommandUnit.h"

#include "Net/UnrealNetwork.h"

UTPSCommandUnit::UTPSCommandUnit()
{
}

void UTPSCommandUnit::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Leader);
}

bool UTPSCommandUnit::IsSupportedForNetworking() const
{
	return true;
}



void UTPSCommandUnit::SetLeader(UTPSCharacterInstance* member)
{
	Leader = member;
}

UTPSCharacterInstance* UTPSCommandUnit::GetLeader() const
{
	return Leader.Get();
}
