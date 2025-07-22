// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterInstance.h"

#include "Character/TPSCharacter.h"
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


void UTPSCharacterInstance::SpawnActor(TSubclassOf<ATPSCharacter> actorTemplate, AActor* spawnPoint)
{
	if (IsValid(SpawnedActor)) { return; }
	if (!IsValid(spawnPoint)) { return; }

	ATPSCharacter* NewActor = GetWorld()->SpawnActor<ATPSCharacter>(actorTemplate,
		spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());

	NewActor->Identity = Identity;
	NewActor->Configuration = Configuration;

	// TODO: Deferred Spawn to allow for setting defaults (Loadout, Identity, etc)

	SpawnedActor = NewActor;
}

void UTPSCharacterInstance::DestroyActor()
{
	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Destroy();
	}
	SpawnedActor = nullptr;
}
