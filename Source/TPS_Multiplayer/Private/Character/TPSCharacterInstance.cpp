// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterInstance.h"

#include "Net/UnrealNetwork.h"

#include "Character/TPSCharacter.h"
#include "Team/TPSTeamInstance.h"

UTPSCharacterInstance::UTPSCharacterInstance()
{
}

void UTPSCharacterInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterID);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, Identity);
	DOREPLIFETIME(ThisClass, Configuration);
	DOREPLIFETIME(ThisClass, Record);

	DOREPLIFETIME(ThisClass, SpawnedActor);
	DOREPLIFETIME(ThisClass, IsAlive);
	DOREPLIFETIME(ThisClass, LastHit);

	DOREPLIFETIME(ThisClass, OwningPlayer);
	DOREPLIFETIME(ThisClass, AssignedUnit);
	DOREPLIFETIME(ThisClass, AssignedArmy);
	DOREPLIFETIME(ThisClass, AssignedTeam);
}

void UTPSCharacterInstance::OnRep_IsAlive()
{
	CharacterInstanceUpdate.Broadcast();
}

void UTPSCharacterInstance::OnRep_OwningPlayer()
{
	CharacterInstanceUpdate.Broadcast();
}

void UTPSCharacterInstance::OnRep_Assignment()
{
	CharacterInstanceUpdate.Broadcast();
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



void UTPSCharacterInstance::AssignToUnit(UTPSCommandUnit* unit)
{
	if (!IsValid(unit)) { return; }

	AssignedUnit = unit;
	AssignedArmy = unit->AssignedArmy;
	AssignedTeam = unit->AssignedTeam;

	CharacterInstanceUpdate.Broadcast();
}

void UTPSCharacterInstance::ClearAssignment()
{
	AssignedUnit = nullptr;
	AssignedArmy = nullptr;
	AssignedTeam = nullptr;

	CharacterInstanceUpdate.Broadcast();
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

	CharacterInstanceUpdate.Broadcast();
}


void UTPSCharacterInstance::Focus()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsFocused = true;

	CharacterInstanceUpdate.Broadcast();
}

void UTPSCharacterInstance::UnFocus()
{
	if (!IsValid(SpawnedActor)) { return; }

	SpawnedActor->IsFocused = false;

	CharacterInstanceUpdate.Broadcast();
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