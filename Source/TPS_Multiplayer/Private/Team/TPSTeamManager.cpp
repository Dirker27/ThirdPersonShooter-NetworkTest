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


void UTPSTeamManager::CreateTeam(ETPSTeamID teamId)
{
	UTPSTeam* team = NewObject<UTPSTeam>(this, UTPSTeam::StaticClass(),
		FName(TPSTeamIdToString(teamId).Append(FString("TEAM-"))));
	team->TeamID = teamId;

	UTPSPlatoon* rootPlatoon = NewObject<UTPSPlatoon>(this, UTPSPlatoon::StaticClass(),
		FName(TPSTeamIdToString(teamId).Append(FString("root"))));
	rootPlatoon->ID.UnitAlias = "ROOT";
	rootPlatoon->ID.UnitNumber = 0;
	rootPlatoon->ID.TeamID = teamId;
	team->RootCollection = rootPlatoon;

	ActiveTeams.Add(teamId, team);
}

void UTPSTeamManager::ConfigureTeam(ETPSTeamID teamId,
	int numSquadsPerPlatoon,
	int numFireteamsPerSquad,
	int numUnitsPerFireteam)
{
	if (UTPSTeam* team = GetTeam(teamId))
	{
		team->Configuration.MaxNumSquadsPerPlatoon = numSquadsPerPlatoon;
		team->Configuration.MaxNumFireteamsPerSquad = numFireteamsPerSquad;
		team->Configuration.MaxNumMembersPerFireteam = numUnitsPerFireteam;

		for (int i = 0; i < numSquadsPerPlatoon; i++)
		{
			FName squadName = FName(TPSTeamIdToString(teamId).Append(FString("-") + FString::FromInt(i)));

			UTPSSquad* squad = NewObject<UTPSSquad>(this, UTPSSquad::StaticClass(), squadName);

			squad->ID.UnitNumber = i;
			squad->ID.UnitAlias = GetAliasForUnitNumber(squad->ID.UnitNumber);
			squad->ID.ParentHierarchyMap.Add(PLATOON, 0);
			team->RootCollection->ChildCollections.Add(squad);

			for (int j = 0; j < numFireteamsPerSquad; j++)
			{
				FName fireteamName = FName(TPSTeamIdToString(teamId).Append(
					FString("-") + FString::FromInt(i)
					+ FString("_") + FString::FromInt(j + 1)));

				UTPSFireTeam* fireteam = NewObject<UTPSFireTeam>(this, UTPSFireTeam::StaticClass(), fireteamName);
				fireteam->ID.UnitNumber = j+1;
				fireteam->ID.UnitAlias = GetAliasForUnitNumber(fireteam->ID.UnitNumber);
				fireteam->ID.ParentHierarchyMap.Add(PLATOON, 0);
				fireteam->ID.ParentHierarchyMap.Add(SQUAD, squad->ID.UnitNumber);
				squad->ChildCollections.Add(fireteam);

				for (int k = 0; k < numUnitsPerFireteam; k++)
				{
					FName unitName = FName(TPSTeamIdToString(teamId).Append(
						FString("-") + FString::FromInt(i)
						+ FString("_") + FString::FromInt(j + 1) 
						+ FString("_") + FString::FromInt(k + 1)));

					UTPSCharacterInstance* unit = NewObject<UTPSCharacterInstance>(this, UTPSCharacterInstance::StaticClass(), unitName);

					unit->Identity->Name = unitName;
					unit->Identity->AssignedUnit = fireteam->ID;
					unit->Identity->SquadRole = (k < 1) ? ETPSSquadRole::Leader : ETPSSquadRole::Rifleman;
					unit->Configuration->BodyType = (k % 2 == 0) ? ETPSCharacterBodyType::Male : ETPSCharacterBodyType::Female;

					fireteam->Members.Add(unit);
				}
			}
		}
	}
}


void UTPSTeamManager::AssignCharacterToTeam(UTPSCharacterInstance* character, ETPSTeamID team)
{
	// TODO
}


void UTPSTeamManager::AssignCharacterToTeamUnit(UTPSCharacterInstance* character, FUnitID unitId)
{
	if (UTPSCommandStructure* unit = GetUnit(unitId))
	{
		unit->Add(character);
		//character->Identity->TeamID = teamId;
		character->Identity->AssignedUnit = unitId;
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

static UTPSCommandStructure* _FindUnit(UTPSCommandStructure* root, FUnitID unitId)
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
		Cast<UTPSCommandStructure>(root->ChildCollections[*unitId.ParentHierarchyMap.Find(childLvl)]),
		unitId);
}
UTPSCommandStructure* UTPSTeamManager::GetUnit(FUnitID unitId)
{
	if (UTPSTeam* team = GetTeam(unitId.TeamID))
	{
		UTPSPlatoon* p = Cast<UTPSPlatoon>(team->RootCollection);
		return _FindUnit(p, unitId);
	}
	return nullptr;
}
