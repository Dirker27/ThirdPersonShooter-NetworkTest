// (C) ToasterCat Studios 2025

#include "Team/TPSTeamInstanceFactory.h"

#include "Character/TPSCharacterInstanceFactory.h"
#include "Game/TPSGameState.h"
#include "Kismet/GameplayStatics.h"

#include "Util/TPSFunctionLibrary.h"

UTPSTeamInstanceFactory::UTPSTeamInstanceFactory()
{
}

void UTPSTeamInstanceFactory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// TODO: Migrate to CommandUnitFactory
static FString GetAliasForUnitNumber(int num)
{
	switch (num)
	{
	case 0:
		return "HQ";
	case 1:
		return "Alpha";
	case 2:
		return "Bravo";
	case 3:
		return "Charlie";
	default:
		return FString::FromInt(num);
	}
}


void UTPSTeamInstanceFactory::CreateTeam(const ETPSTeamID teamId)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	TObjectPtr<UTPSTeamInstance> team = NewObject<UTPSTeamInstance>(
		state,
		UTPSTeamInstance::StaticClass(),
		FName(FString("TEAM-").Append(TPSTeamIdToString(teamId))));
	team->TeamID = teamId;

	state->Teams.Add(team);
	state->AddReplicatedSubObject(team);
}


void UTPSTeamInstanceFactory::ConfigureTeam(const ETPSTeamID teamId, FTPSTeamDefinitionData definition)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSTeamInstance* team = state->GetTeam(teamId))
	{
		team->Configuration = definition.Configuration;

		for (auto def : definition.Armies)
		{
			UTPSArmyInstance* army = NewObject<UTPSArmyInstance>(
				state, 
				UTPSArmyInstance::StaticClass(),
				FName(FString("ARMY-").Append(def->Definition.ArmyID.Guid.ToString())));
			army->AssignToTeam(team);
			_ConfigureArmy(army, def->Definition);

			if (IsValid(def->Definition.RootUnitSchema))
			{
				UTPSCommandUnit* unit = NewObject<UTPSCommandUnit>(state, UTPSCommandUnit::StaticClass());
				unit->UnitNumber = 1;

				_ConfigureUnit(unit, def->Definition.RootUnitSchema->Schema, army);
				army->AssignRootUnit(unit);
			}

			team->Armies.Add(army);
		}
	}
}

// TODO: Migrate to ArmyFactory
void UTPSTeamInstanceFactory::_ConfigureArmy(UTPSArmyInstance* army, FTPSArmyDefinitionData data)
{
	if (!IsValid(army)) { return; }
	UE_LOG(LogTemp, Log, TEXT("Configuring Army[%s]..."), *army->ArmyID.Guid.ToString());

	army->ArmyID = data.ArmyID;
	army->Identity = data.Identity;

	// Index to TPSGameState
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	state->Armies.Add(army);
}


// TODO: Migrate to UnitFactory
void UTPSTeamInstanceFactory::_ConfigureUnit(UTPSCommandUnit* node, FTPSUnitSchemaData schema, UTPSArmyInstance* army)
{
	if (!IsValid(node)) { return; }
	UE_LOG(LogTemp, Log, TEXT("Configuring Unit[%s]..."), *UTPSFunctionLibrary::GetNameForUnitID(node->UnitID));

	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));

	node->UnitLevel = schema.Level;
	node->Schema = schema;
	army->AddUnit(node);

	int unitNum = 1;
	for (auto subUnitSchema : schema.SubUnitDefinitions)
	{
		UTPSCommandUnit* subUnit = NewObject<UTPSCommandUnit>(state, UTPSCommandUnit::StaticClass());
		subUnit->UnitNumber = unitNum++;
		_ConfigureUnit(subUnit, subUnitSchema->Schema, army);
		node->AddSubCollection(subUnit);
	}

	// Index to TPSGameState
	state->TeamUnits.Add(node);
	state->AddReplicatedSubObject(node);
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
		UTPSCharacterInstanceFactory::ConfigureCharacterInstanceForUnitAndRole(instance,
			node, roleDefinition);
		UE_LOG(LogTemp, Log, TEXT("Instantiated/Loaded Character[%s]..."), *instance->CharacterID.Guid.ToString());

		// Assign Loadout
		instance->Loadout = roleDefinition.Loadout;

		// Store
		node->AddMember(instance);
		instance->AssignToUnit(node);

		ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
		state->Characters.Add(instance);
		state->AddReplicatedSubObject(instance);
	}

	for (auto subUnit : node->GetAllSubCollections())
	{
		_PopulateUnit(Cast<UTPSCommandUnit>(subUnit));
	}
}
void UTPSTeamInstanceFactory::PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster)
{
	for (auto member : roster)
	{
		AssignCharacterToTeam(teamId, member);
	}

	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (auto team = state->GetTeam(teamId))
	{
		for (auto army : team->Armies)
		{
			_PopulateUnit(army->GetRootUnit());
		}
	}
}


void UTPSTeamInstanceFactory::AssignCharacterToTeam(ETPSTeamID team, UTPSCharacterInstance* character)
{
	// TODO
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
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	UTPSCharacterInstance* character = NewObject<UTPSCharacterInstance>(state, UTPSCharacterInstance::StaticClass());

	return character;
}
