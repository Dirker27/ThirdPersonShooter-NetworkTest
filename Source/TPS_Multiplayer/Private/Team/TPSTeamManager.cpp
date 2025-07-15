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
	root->ID.UnitLevel = ETPSHierarchicalLevel::ROOT;
	root->ID.UnitNumber = 1;
	root->ID.TeamID = teamId;
	root->Level = ETPSHierarchicalLevel::ROOT;
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
			FName platoonName = FName(TPSTeamIdToString(teamId).Append(FString("_") + FString::FromInt(a)));

			UTPSPlatoon* platoon = NewObject<UTPSPlatoon>(this, UTPSPlatoon::StaticClass(), platoonName);

			platoon->ID.UnitLevel = ETPSHierarchicalLevel::PLATOON;
			platoon->ID.UnitNumber = a+1;
			platoon->ID.TeamID = teamId;
			platoon->ID.Hierarchy.Add(team->RootCollection->ID.UnitLevel, team->RootCollection->ID.UnitNumber);

			team->RootCollection->AddSubCollection(platoon);

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

				squad->Alias = GetAliasForUnitNumber(squad->ID.UnitNumber);
				squad->ID.UnitLevel = ETPSHierarchicalLevel::SQUAD;
				squad->ID.UnitNumber = i+1;
				squad->ID.TeamID = teamId;
				squad->ID.Hierarchy = platoon->ID.Hierarchy;
				squad->ID.Hierarchy.Add(platoon->ID.UnitLevel, platoon->ID.UnitNumber);

				platoon->AddSubCollection(squad);

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

					fireteam->Alias = GetAliasForUnitNumber(fireteam->ID.UnitNumber);
					fireteam->ID.UnitLevel = ETPSHierarchicalLevel::FIRE_TEAM;
					fireteam->ID.UnitNumber = j+1;
					fireteam->ID.TeamID = teamId;
					fireteam->ID.Hierarchy = squad->ID.Hierarchy;
					fireteam->ID.Hierarchy.Add(squad->ID.UnitLevel, squad->ID.UnitNumber);

					squad->AddSubCollection(fireteam);

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

						UTPSCharacterInstance* unit = NewObject<UTPSCharacterInstance>(this, UTPSCharacterInstance::StaticClass(), unitName);

						unit->Identity->UnitID.UnitLevel = ETPSHierarchicalLevel::UNIT;
						unit->Identity->UnitID.UnitNumber = j + 1;
						unit->Identity->UnitID.TeamID = teamId;
						unit->Identity->UnitID.Hierarchy = fireteam->ID.Hierarchy;
						unit->Identity->UnitID.Hierarchy.Add(fireteam->ID.UnitLevel, fireteam->ID.UnitNumber);

						unit->Identity->Name = unitName;
						unit->Identity->SquadRole = (k < 1) ? ETPSSquadRole::Leader : ETPSSquadRole::Rifleman;
						unit->Configuration->BodyType = (k % 2 == 0) ? ETPSCharacterBodyType::Male : ETPSCharacterBodyType::Female;

						fireteam->AddMember(unit);
					}
				}
			}
		}
	}
}

/*UTPSHierarchicalCollection* UTPSTeamManager::ConfigureHierarchicalUnit(ETPSHierarchicalLevel level, int unitNum, UTPSHierarchicalCollection* parent)
{
	FTPSUnitID id = parent.

	UTPSSquad* squad = NewObject<UTPSSquad>(this, UTPSSquad::StaticClass(), squadName);

	return squad;
}*/


void UTPSTeamManager::AssignCharacterToTeam(UTPSCharacterInstance* character, ETPSTeamID team)
{
	// TODO
}


void UTPSTeamManager::AssignCharacterToTeamUnit(UTPSCharacterInstance* character, FTPSUnitID unitId)
{
	if (UTPSCommandStructure* unit = GetUnit(unitId))
	{
		unit->AddMember(character);
		//character->Identity->TeamID = teamId;
		character->Identity->UnitID = unitId;
	}
}


UTPSTeam* UTPSTeamManager::GetTeam(ETPSTeamID teamId)
{
	UTPSTeam** t = ActiveTeams.Find(teamId);
	if (t != nullptr)
	{
		return *t;
	}
	return nullptr;
}

/*static UTPSCommandStructure* _FindUnit(UTPSCommandStructure* root, FTPSUnitID unitId)
{
	if (root->Level == unitId.UnitLevel)
	{
		if (root->ID.UnitNumber == unitId.UnitNumber)
		{
			return root;
		}
		return nullptr;
	}

	ETPSHierarchicalLevel childLvl = ETPSHierarchicalLevel(root->Level - 1);
	return _FindUnit(
		Cast<UTPSCommandStructure>(root->ChildCollections[*unitId.Hierarchy.Find(childLvl)]),
		unitId);
}*/
UTPSCommandStructure* UTPSTeamManager::GetUnit(FTPSUnitID unitId)
{
	/*if (UTPSTeam* team = GetTeam(unitId.TeamID))
	{
		UTPSPlatoon* p = Cast<UTPSPlatoon>(team->RootCollection);
		return _FindUnit(p, unitId);
	}*/
	return nullptr;
}
