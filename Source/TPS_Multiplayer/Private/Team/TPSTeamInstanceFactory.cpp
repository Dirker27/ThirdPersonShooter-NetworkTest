// (C) ToasterCat Studios 2025

#include "Team/TPSTeamInstanceFactory.h"

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

void UTPSTeamInstanceFactory::ConfigureTeam(const ETPSTeamID teamId, FTPSTeamConfiguration configuration)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSTeamInstance* team = state->GetTeam(teamId))
	{
		team->Configuration = configuration;

		UTPSCommandStructure* unit = NewObject<UTPSCommandStructure>(state, UTPSCommandStructure::StaticClass());
		unit->UnitID.TeamID = teamId;
		unit->UnitID.UnitNumber = 1;
		_ConfigureUnit(unit, configuration, configuration.HighestSupportedCommandLevel);

		team->RootUnit = unit;
	}
}


// TODO: Migrate to UnitFactory
void UTPSTeamInstanceFactory::_ConfigureUnit(UTPSCommandStructure* node, FTPSTeamConfiguration configuration,
	ETPSHierarchicalLevel level)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));

	node->UnitID.UnitLevel = level;
	//node->UnitID.UnitNumber = unitNumber;

	int memberCount = (configuration.MemberCapacityMap.Find(level))
		? *configuration.MemberCapacityMap.Find(level)
		: 0;
	int subUnitCount = (configuration.SubUnitCapacityMap.Find(level))
		? *configuration.SubUnitCapacityMap.Find(level)
		: 0;
	node->Configuration.MaxNumMembers = memberCount;
	node->Configuration.MaxNumSubCollections = subUnitCount;
	node->Configuration.MemberLoadoutMap = configuration.MemberLoadoutMap;


	if (level > ETPSHierarchicalLevel::FIRE_TEAM) 
	{
		for (int i = 0; i < subUnitCount; i++)
		{
			UTPSCommandStructure* subUnit = NewObject<UTPSCommandStructure>(state, UTPSCommandStructure::StaticClass());
			_ConfigureUnit(subUnit, configuration, _TPSLevel_Down(level));
			node->AddSubCollection(subUnit);
		}
	}

	if (level == ETPSHierarchicalLevel::SQUAD)
	{
		node->Alias = GetAliasForUnitNumber(node->UnitID.UnitNumber);
	}

	// Index to TPSGameState
	state->TeamUnits.Add(node);
	state->AddReplicatedSubObject(node);
}



// TODO: Derive unit role/rank from TeamConfig
void UTPSTeamInstanceFactory::_PopulateUnit(UTPSCommandStructure* node)
{
	if (!IsValid(node)) { return; }

	// Instantiate and map CharacterInstances -> Unit/Team
	for (int i = 0; i < node->Configuration.MaxNumMembers; i++)
	{
		//- Instantiate CharacterInstances -----------------------=
		//
		// TODO: [PC-173] Construct CharacterInstances from CharacterInstanceFactory (supplied by GameMode/TeamConfiguration)
		//
		UTPSCharacterInstance* instance = _NewCharacter();
		node->AddMember(instance);
		//
		instance->OpId.Guid = FGuid::NewGuid();
		instance->OpId.FirstName = "Karl";
		instance->OpId.Callsign = "Callsign";
		instance->OpId.LastName = UTPSFunctionLibrary::GetNameForUnitID(
			instance->OpId.UnitID);
		instance->OpId.CharacterBodyType = 
			(FMath::RandRange(0, 1) > 0)
				? ETPSCharacterBodyType::Male
				: ETPSCharacterBodyType::Female;


		//- Assign Squad Role from Schema -----------------------=
		//
		// TODO: [PC-174] Derive this from UnitSchema in FTPSCommandUnitConfiguration
		//
		instance->OpId.SquadRole = ETPSSquadRole::Rifleman;
		if (i == 2) { instance->OpId.SquadRole = ETPSSquadRole::AutomaticRifleman; }
		//
		instance->OpId.Rank = ETPSCharacterRank::Private;
		if (i == 0)
		{
			instance->OpId.SquadRole = ETPSSquadRole::Leader;

			switch (node->UnitID.UnitLevel) {
			case ETPSHierarchicalLevel::FIRE_TEAM:
				instance->OpId.Rank = ETPSCharacterRank::Corporal;
				break;
			case ETPSHierarchicalLevel::SQUAD:
				instance->OpId.Rank = ETPSCharacterRank::Sergeant;
				break;
			case ETPSHierarchicalLevel::PLATOON:
				instance->OpId.Rank = ETPSCharacterRank::Lieutenant;
				break;
			case ETPSHierarchicalLevel::COMPANY:
				instance->OpId.Rank = ETPSCharacterRank::Captain;
				break;
			}
			node->SetLeader(instance);
		}

		// Assign Loadout based on SquadRole
		if (auto l = node->Configuration.MemberLoadoutMap
				.Find(instance->OpId.SquadRole))
		{
			instance->Loadout = *l;
		}

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
		_PopulateUnit(Cast<UTPSCommandStructure>(subUnit));
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
		_PopulateUnit(team->RootUnit);
	}
}

void UTPSTeamInstanceFactory::ActivateCharacter(UTPSCharacterInstance* instance)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	if (UTPSTeamInstance* team = state->GetTeam(instance->OpId.UnitID.TeamID))
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
	if (UTPSCommandStructure* existingUnit = state->GetUnit(character->OpId.UnitID))
	{
		existingUnit->RemoveMember(character->OpId.UnitID.UnitNumber);
	}

	if (UTPSCommandStructure* unit = state->GetUnit(unitId))
	{
		unit->AddMember(character);
	}
}



// TODO: Generate default characters from a CharacterFactory component
UTPSCharacterInstance* UTPSTeamInstanceFactory::_NewCharacter()
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));
	UTPSCharacterInstance* character = NewObject<UTPSCharacterInstance>(state, UTPSCharacterInstance::StaticClass());
	return character;
}
