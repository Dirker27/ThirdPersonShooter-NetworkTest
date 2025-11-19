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
				FName(FString("ARMY-").Append(TPSTeamIdToString(teamId))));

			UTPSCommandUnit* unit = NewObject<UTPSCommandUnit>(state, UTPSCommandUnit::StaticClass());
			unit->UnitID.TeamID = teamId;
			unit->UnitID.UnitNumber = 1;
			_ConfigureUnit(unit, def->Definition.RootUnitSchema->Schema);

			army->RootUnit = unit;
			team->Armies.Add(army);
			state->Armies.Add(army);
		}
	}
}


// TODO: Migrate to UnitFactory
void UTPSTeamInstanceFactory::_ConfigureUnit(UTPSCommandUnit* node, FTPSUnitSchemaData schema)
{
	if (!IsValid(node)) { return; }

	UE_LOG(LogTemp, Log, TEXT("Configuring Unit[%s]..."), *UTPSFunctionLibrary::GetNameForUnitID(node->UnitID));

	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));

	node->UnitID.UnitLevel = schema.Level;
	node->Schema = schema;

	for (auto subUnitSchema : schema.SubUnitDefinitions)
	{
		UTPSCommandUnit* subUnit = NewObject<UTPSCommandUnit>(state, UTPSCommandUnit::StaticClass());
		_ConfigureUnit(subUnit, subUnitSchema->Schema);
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
			node->UnitID, roleDefinition);

		// Assign Loadout
		instance->Loadout = roleDefinition.Loadout;

		// Store
		node->AddMember(instance);
		ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
		if (auto team = state->GetTeam(node->UnitID.TeamID))
		{
			team->Members.Add(instance);
			state->Characters.Add(instance);
			state->AddReplicatedSubObject(instance);
		}
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
			_PopulateUnit(army->RootUnit);
		}
	}
}

void UTPSTeamInstanceFactory::ActivateCharacter(UTPSCharacterInstance* instance)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSTeamInstance* team = state->GetTeam(instance->Identity.UnitID.TeamID))
	{
		team->ActiveMembers.Add(instance);
		/*team->ActiveMembers.Add(
			FTPSUnitID::HashUnitIdentifier(instance->Identity->UnitID),
			instance);*/
	}
}


void UTPSTeamInstanceFactory::AssignCharacterToTeam(ETPSTeamID team, UTPSCharacterInstance* character)
{
	// TODO
}


void UTPSTeamInstanceFactory::AssignCharacterToTeamUnit(FTPSUnitID unitId, UTPSCharacterInstance* character)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSCommandUnit* existingUnit = state->GetUnit(character->Identity.UnitID))
	{
		existingUnit->RemoveMember(character->Identity.UnitID.UnitNumber);
	}

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
