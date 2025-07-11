// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterInstance.h"

#include "Character/TPSCharacterRecord.h"
#include "Net/UnrealNetwork.h"

UTPSCharacterInstance::UTPSCharacterInstance()
{
	Identity = CreateDefaultSubobject<UTPSCharacterIdentity>(TEXT("Identity"));
	Configuration = CreateDefaultSubobject<UTPSCharacterConfiguration>(TEXT("Configuration"));
	Record = CreateDefaultSubobject<UTPSCharacterRecord>(TEXT("Record"));
}

void UTPSCharacterInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
}