// (C) ToasterCat Studios 2025

#include "Team/TPSTeamInstanceFactory.h"

#include "Character/TPSCharacterInstanceFactory.h"
#include "Game/TPSGameState.h"
#include "Kismet/GameplayStatics.h"

#include "Util/TPSFunctionLibrary.h"

UTPSTeamInstanceFactory::UTPSTeamInstanceFactory()
{
}

void UTPSTeamInstanceFactory::CreateTeam(const ETPSTeamID teamId)
{
	TObjectPtr<UTPSTeamInstance> team = NewObject<UTPSTeamInstance>(
		State(),
		UTPSTeamInstance::StaticClass(),
		FName(FString("TEAM-").Append(TPSTeamIdToString(teamId))));
	team->TeamID = teamId;

	State()->AddReplicatedSubObject(team);
	State()->Teams.Add(team);
}


void UTPSTeamInstanceFactory::ConfigureTeam(const ETPSTeamID teamId, FTPSTeamDefinitionData definition)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSTeamInstance* team = state->GetTeam(teamId))
	{
		team->Configuration = definition.Configuration;

		for (auto def : definition.Armies)
		{
			// TODO: Migrate to ArmyInstanceFactory [PC-238]
			UTPSArmyInstance* army = NewObject<UTPSArmyInstance>(
				state, 
				UTPSArmyInstance::StaticClass(),
				FName(FString("ARMY-").Append(def->Definition.ArmyID.Guid.ToString())));
			army->ArmyID = def->Definition.ArmyID;
			army->AssignToTeam(team);
			_ConfigureArmy(army, def->Definition);

			team->AddArmy(army);
		}
	}
}

// TODO: Migrate to ArmyFactory [PC-238]
void UTPSTeamInstanceFactory::_ConfigureArmy(UTPSArmyInstance* army, FTPSArmyDefinitionData data)
{
	if (!IsValid(army)) { return; }
	UE_LOG(LogTemp, Log, TEXT("Configuring Army[%s]..."), *army->ArmyID.ToString());

	army->Identity = data.Identity;

	if (IsValid(data.RootUnitSchema))
	{
		UTPSCommandUnit* unit = NewObject<UTPSCommandUnit>(State(), UTPSCommandUnit::StaticClass());
		unit->UnitID.Guid = FGuid::NewGuid();
		unit->UnitNumber = 1;
		unit->Identity.BannerColor = army->Identity->BannerColor;
		_ConfigureUnit(unit, data.RootUnitSchema->Schema, army);
		army->AssignRootUnit(unit);

		State()->AddReplicatedSubObject(unit);
		State()->TeamUnits.Add(unit);
	}

	// Index to TPSGameState
	State()->AddReplicatedSubObject(army);
	State()->Armies.Add(army);
	UE_LOG(LogTemp, Log, TEXT("Army[%s] instantiated."), *army->ArmyID.ToString());
}


// TODO: Migrate to UnitFactory [PC-238]
void UTPSTeamInstanceFactory::_ConfigureUnit(UTPSCommandUnit* node, FTPSUnitSchemaData schema, UTPSArmyInstance* army)
{
	if (!IsValid(node)) { return; }
	UE_LOG(LogTemp, Log, TEXT("Instantiating Unit[%s]..."), *UTPSFunctionLibrary::GetNameForUnitID(node->UnitID));

	node->UnitLevel = schema.Level;
	node->Identity.Alias = UTPSUnitInstanceFactory::RandomSquadName();
	node->Schema = schema;
	army->AddUnit(node);

	int unitNum = 1;
	for (auto subUnitSchema : schema.SubUnitDefinitions)
	{
		UTPSCommandUnit* subUnit = NewObject<UTPSCommandUnit>(State(), UTPSCommandUnit::StaticClass());
		subUnit->UnitID.Guid = FGuid::NewGuid();
		subUnit->UnitNumber = unitNum++;
		subUnit->Identity.BannerColor = node->Identity.BannerColor;
		_ConfigureUnit(subUnit, subUnitSchema->Schema, army);
		node->AddSubCollection(subUnit);
	}

	// Index to TPSGameState
	State()->AddReplicatedSubObject(node);
	State()->TeamUnits.Add(node);
	UE_LOG(LogTemp, Log, TEXT("Unit[%s] instantiated."), *node->UnitID.ToString());
}


void UTPSTeamInstanceFactory::_PopulateUnit(UTPSCommandUnit* node)
{
	if (!IsValid(node)) { return; }
	UE_LOG(LogTemp, Log, TEXT("Populating Unit[%s]..."), *UTPSFunctionLibrary::GetNameForUnitID(node->UnitID));

	for (auto roleDefinition : node->Schema.MemberDefinitions)
	{
		// Instantiate
		UTPSCharacterInstance* instance = _NewCharacter();

		// Configure (via CharacterFactory)
		CharacterFactory->ConfigureCharacterInstanceForUnitAndRole(instance,
			node, roleDefinition);
		UE_LOG(LogTemp, Log, TEXT("Instantiating CharacterInstance[%s]..."), *instance->CharacterID.Guid.ToString());

		// Assign Loadout
		instance->Loadout = roleDefinition.Loadout;

		// Store
		node->AddMember(instance);
		instance->AssignToUnit(node);
		if (instance->Identity.SquadRole == Leader) {
			node->SetLeader(instance);
		}

		State()->Characters.Add(instance);
		State()->AddReplicatedSubObject(instance);
		UE_LOG(LogTemp, Log, TEXT("CharacterInstance[%s] instantiated."), *instance->CharacterID.Guid.ToString());
	}
	if (!IsValid(node->GetLeader()) && !node->GetAllMembers().IsEmpty())
	{
		node->SetLeader(node->GetAllMembers()[0]);
	}

	for (auto subUnit : node->GetAllSubCollections())
	{
		_PopulateUnit(Cast<UTPSCommandUnit>(subUnit));
	}
}
void UTPSTeamInstanceFactory::PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster)
{
	// TODO: Add provided Characters to assigned slots in roster

	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (auto team = state->GetTeam(teamId))
	{
		for (auto army : team->GetArmies())
		{
			_PopulateUnit(army->GetRootUnit());
		}
	}
}


void UTPSTeamInstanceFactory::AssignCharacterToTeamUnit(FTPSUnitID unitId, UTPSCharacterInstance* character)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSCommandUnit* unit = state->GetUnit(unitId))
	{
		unit->AddMember(character);
	}
}



UTPSCharacterInstance* UTPSTeamInstanceFactory::_NewCharacter()
{
	UTPSCharacterInstance* character = NewObject<UTPSCharacterInstance>(State(), UTPSCharacterInstance::StaticClass());

	return character;
}


ATPSGameState* UTPSTeamInstanceFactory::State() const
{
	return Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
}