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


	/*ATPSCharacter* characterActor = GetWorld()->SpawnActor<ATPSCharacter>(actorTemplate,
		spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());*/

	ATPSCharacter* characterActor = GetWorld()->SpawnActorDeferred<ATPSCharacter>(
		actorTemplate,
		spawnPoint->GetTransform());

	characterActor->Identity = Identity;
	characterActor->Configuration = Configuration;
	characterActor->EquipmentManager->Loadout = Loadout;

	characterActor->FinishSpawning(spawnPoint->GetTransform(), /*bIsDefaultTransform=*/ true);

	SpawnedActor = characterActor;
}

void UTPSCharacterInstance::DestroyActor()
{
	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Destroy();
	}
	SpawnedActor = nullptr;
}
