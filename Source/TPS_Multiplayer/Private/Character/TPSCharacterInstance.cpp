// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterInstance.h"

#include "Net/UnrealNetwork.h"

#include "Character/TPSCharacter.h"
#include "Character/TPSCharacterRecord.h"

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

	DOREPLIFETIME(ThisClass, IsAlive);
	DOREPLIFETIME(ThisClass, SpawnedActor);
}

bool UTPSCharacterInstance::IsSupportedForNetworking() const
{
	return true;
}



void UTPSCharacterInstance::SpawnActor(TSubclassOf<ATPSCharacter> actorTemplate, AActor* spawnPoint)
{
	if (IsValid(SpawnedActor)) { return; }
	if (!IsValid(spawnPoint)) { return; }

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



void UTPSCharacterInstance::Die()
{
	IsAlive = false;

	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Die();
	}
}


void UTPSCharacterInstance::Focus()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsFocused = true;
}

void UTPSCharacterInstance::UnFocus()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsFocused = false;
}


void UTPSCharacterInstance::ShowDebug()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsDebugEnabled = true;
}

void UTPSCharacterInstance::HideDebug()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsDebugEnabled = false;
}