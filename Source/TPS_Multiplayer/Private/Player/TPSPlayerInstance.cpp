// (C) ToasterCat Studios 2025

#include "Player/TPSPlayerInstance.h"

#include "Net/UnrealNetwork.h"

UTPSPlayerInstance::UTPSPlayerInstance()
{
}

void UTPSPlayerInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerID);
	DOREPLIFETIME(ThisClass, State);
}

