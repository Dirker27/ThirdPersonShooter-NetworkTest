// (C) ToasterCat Studios 2025

#include "Game/TPSSpawnPool.h"

UTPSSpawnPool::UTPSSpawnPool()
{
	
}


void UTPSSpawnPool::AddSpawnPointToPool(ATPSSpawnPoint* spawn)
{
	SpawnPoints.Add(spawn);
}

ATPSSpawnPoint* UTPSSpawnPool::FindFirstAvailableSpawnPoint()
{
	for (auto sp : SpawnPoints)
	{
		if (sp->IsSpawnAvailable())
		{
			return sp;
		}
	}
	return nullptr;
}

ATPSSpawnPoint* UTPSSpawnPool::FindBestSpawnPointForSquadRole(const ETPSSquadRole role)
{
	for (auto sp : SpawnPoints)
	{
		if (sp->PreferredSquadRoleToSpawn == role && sp->IsSpawnAvailable())
		{
			return sp;
		}
	}
	return FindFirstAvailableSpawnPoint();
}

// TODO: Something WAY better than this...
ATPSSpawnPoint* UTPSSpawnPool::FindBestSpawnPointForUnitAndSquadRole(const FTPSUnitID unitId, const ETPSSquadRole role)
{
	ATPSSpawnPoint* bestSpawn = FindFirstAvailableSpawnPoint();
	ETPSHierarchicalLevel bestLevel = ETPSHierarchicalLevel::ROOT;

	for (auto sp : SpawnPoints)
	{
		if (bestLevel < sp->AssignedUnit.UnitLevel) { continue;	}

		// Exact Fit - return immediately
		if (sp->AssignedUnit == unitId)
		{
			if (sp->PreferredSquadRoleToSpawn == role && sp->IsSpawnAvailable())
			{
				return sp;
			}
		}

		// Near Fit - keep scanning
		if (auto unitNumber = unitId.Hierarchy.Find(sp->AssignedUnit.UnitLevel))
		{
			if (*unitNumber == sp->AssignedUnit.UnitNumber
				&& sp->PreferredSquadRoleToSpawn == role 
				&& sp->IsSpawnAvailable())
			{
				bestSpawn = sp;
				bestLevel = sp->AssignedUnit.UnitLevel;				
			}
		}
	}

	return bestSpawn;
}
