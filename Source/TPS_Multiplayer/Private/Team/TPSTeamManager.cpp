// (C) ToasterCat Studios 2025

#include "Team/TPSTeamManager.h"

#include "Team/TPSFireTeam.h"
#include "Team/TPSPlatoon.h"
#include "Team/TPSSquad.h"

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

/*static FString GetNameForUnitID(FTPSUnitID unitId)
{
	FString name = TPSTeamIdToString(unitId.TeamID);

	ETPSHierarchicalLevel currentLevel = unitId.UnitLevel;
	for (int i = unitId.UnitLevel; i >=0; i--)
	{
		currentLevel = static_cast<ETPSHierarchicalLevel>(i);

		name.Append()
	}

	FString n = name.Append("");

	

	hierarchy

	return name;
}*/


void UTPSTeamManager::CreateTeam(ETPSTeamID teamId)
{
	UTPSTeam* team = NewObject<UTPSTeam>(this, UTPSTeam::StaticClass(),
		FName(TPSTeamIdToString(teamId).Append(FString("TEAM-"))));
	team->TeamID = teamId;

	UTPSCommandStructure* root = NewObject<UTPSCommandStructure>(this, UTPSCommandStructure::StaticClass(),
		FName(TPSTeamIdToString(teamId).Append(FString("root"))));

	root->UnitID.UnitLevel = ETPSHierarchicalLevel::ROOT;
	root->UnitID.UnitNumber = 1;
	root->UnitID.TeamID = teamId;
	root->UnitID.Hierarchy.Add(ETPSHierarchicalLevel::ROOT, 0);

	team->RootCollection = root;

	ActiveTeams.Add(teamId, team);
}



void UTPSTeamManager::ConfigureTeam(ETPSTeamID teamId, FTPSTeamConfiguration configuration)
{
	if (UTPSTeam* team = GetTeam(teamId))
	{
		team->Configuration = configuration;

		// Platoon
		int platoonCount = (configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::SQUAD))
			? *configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::SQUAD)
			: 0;
		for (int a = 0; a < platoonCount; a++) {
			FName platoonName = FName(TPSTeamIdToString(teamId).Append(
				FString("_") + FString::FromInt(a)));

			UTPSPlatoon* platoon = NewObject<UTPSPlatoon>(this, UTPSPlatoon::StaticClass(), platoonName);
			team->RootCollection->AddSubCollection(platoon);

			platoon->GrantLeader(NewCharacter(FName(platoonName.ToString() + "_LEAD")));

			// Squad
			int squadCount = (configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::SQUAD))
				? *configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::SQUAD)
				: 0;
			for (int i = 0; i < squadCount; i++)
			{
				FName squadName = FName(TPSTeamIdToString(teamId).Append(
					FString("_") + FString::FromInt(a)
					+ FString("-") + FString::FromInt(i)));

				UTPSSquad* squad = NewObject<UTPSSquad>(this, UTPSSquad::StaticClass(), squadName);
				platoon->AddSubCollection(squad);
				squad->Alias = GetAliasForUnitNumber(squad->UnitID.UnitNumber);

				squad->GrantLeader(NewCharacter(FName(squadName.ToString() + "_LEAD")));

				// FireTeam
				int fireteamCount = (configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::FIRE_TEAM))
					? *configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::FIRE_TEAM)
					: 0;
				for (int j = 0; j < fireteamCount; j++)
				{
					FName fireteamName = FName(TPSTeamIdToString(teamId).Append(
						FString("_") + FString::FromInt(a)
						+ FString("-") + FString::FromInt(i)
						+ FString("_") + FString::FromInt(j + 1)));

					UTPSFireTeam* fireteam = NewObject<UTPSFireTeam>(this, UTPSFireTeam::StaticClass(), fireteamName);
					squad->AddSubCollection(fireteam);

					fireteam->GrantLeader(NewCharacter(FName(fireteamName.ToString() + "_LEAD")));

					// Unit
					int unitCount = (configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::UNIT))
						? *configuration.LevelCapacityMap.Find(ETPSHierarchicalLevel::UNIT)
						: 0;
					for (int k = 0; k < unitCount; k++)
					{
						FName unitName = FName(TPSTeamIdToString(teamId).Append(
							FString("_") + FString::FromInt(a)
							+ FString("-") + FString::FromInt(i)
							+ FString("_") + FString::FromInt(j + 1) 
							+ FString("_") + FString::FromInt(k + 1)));

						UTPSCharacterInstance* unit = NewCharacter(unitName);
						unit->Configuration->BodyType = (k % 2 == 0) ? ETPSCharacterBodyType::Male : ETPSCharacterBodyType::Female;

						unit->Identity->SquadRole = (k % 2 == 0)
							? ETPSSquadRole::Rifleman
							: ETPSSquadRole::AutomaticRifleman;
						fireteam->AddMember(unit);
					}
				}
			}
		}
	}
}


void UTPSTeamManager::AssignCharacterToTeam(UTPSCharacterInstance* character, ETPSTeamID team)
{
	// TODO
}


void UTPSTeamManager::AssignCharacterToTeamUnit(UTPSCharacterInstance* character, FTPSUnitID unitId)
{
	if (UTPSCommandStructure* unit = GetUnit(unitId))
	{
		unit->AddMember(character);
		//character->Identity->UnitID = unitId;
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
		return _GetUnit(unitId, t->RootCollection);
	}
	return nullptr;
}

UTPSCommandStructure* UTPSTeamManager::_GetUnit(FTPSUnitID unitId, UTPSCommandStructure* node)
{
	if (!IsValid(node)) { return nullptr; }
	ETPSHierarchicalLevel currentLevel = node->UnitID.UnitLevel;

	if (unitId.UnitLevel > currentLevel)
	{
		return nullptr;
	}

	if (unitId.UnitLevel < currentLevel) {
		ETPSHierarchicalLevel targetLevel = LevelDown(currentLevel);

		if (uint8* targetUnitNumber = unitId.Hierarchy.Find(targetLevel))
		{
			return _GetUnit(unitId, Cast<UTPSCommandStructure>(node->GetChildUnit(targetLevel, *targetUnitNumber)));
		}
		return nullptr;
	}

	return (unitId.UnitNumber == node->UnitID.UnitNumber)
		? node
		: nullptr;
}

UTPSCharacterInstance* UTPSTeamManager::NewCharacter(FName name)
{
	UTPSCharacterInstance* unit = NewObject<UTPSCharacterInstance>(this, UTPSCharacterInstance::StaticClass(), name);
	unit->Identity->Name = name;
	unit->Identity->SquadRole = ETPSSquadRole::Leader;

	return unit;
}
