// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterInstance.h"

#include "Net/UnrealNetwork.h"

#include "Character/TPSCharacter.h"
#include "Team/TPSTeamInstance.h"

UTPSCharacterInstance::UTPSCharacterInstance()
{
	//Identity = CreateDefaultSubobject<UTPSCharacterIdentity>(TEXT("Identity"));
	//Configuration = CreateDefaultSubobject<UTPSCharacterConfiguration>(TEXT("Configuration"));
	//Record = CreateDefaultSubobject<UTPSCharacterRecord>(TEXT("Record"));
}

void UTPSCharacterInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, Identity);
	DOREPLIFETIME(ThisClass, Configuration);
	DOREPLIFETIME(ThisClass, Record);

	DOREPLIFETIME(ThisClass, SpawnedActor);
	DOREPLIFETIME(ThisClass, IsAlive);
	DOREPLIFETIME(ThisClass, LastHit);
}



void UTPSCharacterInstance::SpawnActor(TSubclassOf<ATPSCharacter> actorTemplate, AActor* spawnPoint)
{
	if (IsValid(SpawnedActor)) { return; }
	if (!IsValid(spawnPoint)) { return; }

	ATPSCharacter* characterActor = GetWorld()->SpawnActorDeferred<ATPSCharacter>(
		actorTemplate,
		spawnPoint->GetTransform());

	characterActor->EquipmentManager->Loadout = Loadout;
	characterActor->BindToCharacterInstance(this);

	characterActor->FinishSpawning(spawnPoint->GetTransform(), /*bIsDefaultTransform=*/ true);
	characterActor->SpawnDefaultController();

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


ETPSTeamID UTPSCharacterInstance::GetAssignedTeamID() const
{
	if (IsValid(AssignedTeam.Get()))
	{
		return AssignedTeam.Get()->TeamID;
	}
	return ETPSTeamID::UNAFFILIATED;
}




void UTPSCharacterInstance::Die()
{
	IsAlive = false;

	// Extend death to Actor
	//   (may have already fired if Character-driven death)
	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Die();
	}

	NotifyDisplayWidgets.Broadcast();
}


void UTPSCharacterInstance::Focus()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsFocused = true;

	NotifyDisplayWidgets.Broadcast();
}

void UTPSCharacterInstance::UnFocus()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsFocused = false;

	NotifyDisplayWidgets.Broadcast();
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