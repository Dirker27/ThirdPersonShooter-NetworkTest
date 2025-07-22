// (C) ToasterCat Studios 2025

#include "Team/TPSTeamManager.h"

#include "Team/TPSFireTeam.h"
#include "Team/TPSPlatoon.h"
#include "Team/TPSSquad.h"
#include "Util/TPSFunctionLibrary.h"

UTPSTeamManager::UTPSTeamManager()
{
}

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


void UTPSTeamManager::CreateTeam(ETPSTeamID teamId)
{
	UTPSTeam* team = NewObject<UTPSTeam>(this, UTPSTeam::StaticClass(),
		FName(TPSTeamIdToString(teamId).Append(FString("TEAM-"))));
	team->TeamID = teamId;

	ActiveTeams.Add(teamId, team);
}

void UTPSTeamManager::ConfigureTeam(ETPSTeamID teamId, FTPSTeamConfiguration configuration)
{
	if (UTPSTeam* team = GetTeam(teamId))
	{
		team->Configuration = configuration;

		UTPSCommandStructure* unit = NewObject<UTPSCommandStructure>(this, UTPSCommandStructure::StaticClass());
		unit->UnitID.TeamID = teamId;
		unit->UnitID.UnitNumber = 1;
		_ConfigureUnit(unit, configuration, configuration.HighestSupportedCommandLevel);

		team->RootCollection = unit;
	}
}



void UTPSTeamManager::_ConfigureUnit(UTPSCommandStructure* node, FTPSTeamConfiguration configuration,
	ETPSHierarchicalLevel level)
{
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


	if (level > ETPSHierarchicalLevel::FIRE_TEAM) 
	{
		for (int i = 0; i < subUnitCount; i++)
		{
			UTPSCommandStructure* subUnit = NewObject<UTPSCommandStructure>(this, UTPSCommandStructure::StaticClass());
			_ConfigureUnit(subUnit, configuration, _TPSLevel_Down(level));
			node->AddSubCollection(subUnit);
		}
	}


	if (level == ETPSHierarchicalLevel::SQUAD)
	{
		node->Alias = GetAliasForUnitNumber(node->UnitID.UnitNumber);
	}


	if (auto team = GetTeam(node->UnitID.TeamID))
	{
		team->UnitsById.Add(FTPSUnitID::HashUnitIdentifier(node->UnitID), node);
	}
}


void UTPSTeamManager::_PopulateUnit(UTPSCommandStructure* node)
{
	//if (!IsValid(node)) { return; }

	for (int i = 0; i < node->Configuration.MaxNumMembers; i++)
	{
		UTPSCharacterInstance* instance = _NewCharacter();
		node->AddMember(instance);

		instance->Identity->Name = FName(UTPSFunctionLibrary::GetNameForUnitID(
			instance->Identity->UnitID));

		if (i == 0)
		{
			instance->Identity->SquadRole = ETPSSquadRole::Leader;

			switch (node->UnitID.UnitLevel) {
			case ETPSHierarchicalLevel::FIRE_TEAM:
				instance->Identity->Rank = ETPSCharacterRank::Corporal;
				break;
			case ETPSHierarchicalLevel::SQUAD:
				instance->Identity->Rank = ETPSCharacterRank::Sergeant;
				break;
			case ETPSHierarchicalLevel::PLATOON:
				instance->Identity->Rank = ETPSCharacterRank::Lieutenant;
				break;
			case ETPSHierarchicalLevel::COMPANY:
				instance->Identity->Rank = ETPSCharacterRank::Captain;
				break;
			}
			node->SetLeader(instance);
		}
	}

	for (auto subUnit : node->GetAllChildCollections())
	{
		_PopulateUnit(Cast<UTPSCommandStructure>(subUnit));
	}
}
void UTPSTeamManager::PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster)
{
	for (auto member : roster)
	{
		AssignCharacterToTeam(teamId, member);
	}

	if (auto team = GetTeam(teamId))
	{
		_PopulateUnit(team->RootCollection);
	}
}



void UTPSTeamManager::AssignCharacterToTeam(ETPSTeamID team, UTPSCharacterInstance* character)
{
	// TODO
}


void UTPSTeamManager::AssignCharacterToTeamUnit(FTPSUnitID unitId, UTPSCharacterInstance* character)
{
	if (UTPSCommandStructure* existingUnit = GetUnit(character->Identity->UnitID))
	{
		existingUnit->RemoveMember(character);
	}

	if (UTPSCommandStructure* unit = GetUnit(unitId))
	{
		unit->AddMember(character);
	}
}


UTPSTeam* UTPSTeamManager::GetTeam(ETPSTeamID teamId)
{
	TObjectPtr<UTPSTeam>* t = ActiveTeams.Find(teamId);
	if (t != nullptr)
	{
		return *t;
	}
	return nullptr;
}

UTPSCommandStructure* UTPSTeamManager::GetUnit(FTPSUnitID unitId)
{
	if (auto t = GetTeam(unitId.TeamID))
	{
		if (auto unit = t->UnitsById.Find(FTPSUnitID::HashUnitIdentifier(unitId)))
		{
			return *unit;
		}

		return Cast<UTPSCommandStructure>(t->RootCollection->GetChildCollection(unitId));
	}
	return nullptr;
}

int UTPSTeamManager::CountUnitMembers(FTPSUnitID unitId)
{
	if (auto unit = GetUnit(unitId))
	{
		return _CountUnitMembers(unit);
	}
	return 0;
}



int UTPSTeamManager::_CountUnitMembers(UTPSHierarchicalCollection* node)
{
	int count = 0;

	for (auto member : node->GetAllMembers())
	{
		if (member->IsAlive)
		{
			count++;
		}
	}

	for (auto subUnit : node->GetAllChildCollections())
	{
		count += _CountUnitMembers(subUnit);
	}

	return count;
}



// TODO: Generate default characters from a CharacterFactory component
UTPSCharacterInstance* UTPSTeamManager::_NewCharacter()
{
	UTPSCharacterInstance* unit = NewObject<UTPSCharacterInstance>(this, UTPSCharacterInstance::StaticClass());
	return unit;
}
