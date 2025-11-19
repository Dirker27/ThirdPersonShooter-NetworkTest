// (C) ToasterCat Studios 2025

#include "World/TPSSpawnPool.h"

UTPSSpawnPool::UTPSSpawnPool()
{
	SpawnPoints.Empty();
	SpawnPointsById.Empty();
}


void UTPSSpawnPool::AddSpawnPointToPool(ATPSSpawnPoint* spawn)
{
	if (IsValid(spawn)) {
		SpawnPoints.Add(spawn);
		SpawnPointsById.Add(FTPSUnitID::HashUnitIdentifier(spawn->AssignedUnit), spawn);
	}
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

ATPSSpawnPoint* UTPSSpawnPool::FindBestSpawnPointForSquadRole(const ETPSUnitRole role)
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


ATPSSpawnPoint* UTPSSpawnPool::FindBestSpawnPointForUnitAndSquadRole(const FTPSUnitID unitId, const ETPSUnitRole role)
{
	// Exact fit by Unit ID - return immediately
	int unitHash = FTPSUnitID::HashUnitIdentifier(unitId);
	if (SpawnPointsById.Contains(unitHash))
	{
		auto spawn = *SpawnPointsById.Find(unitHash);
		if (spawn->IsSpawnAvailable()) {
			return spawn;
		}
	}

	ATPSSpawnPoint* bestSpawn = FindFirstAvailableSpawnPoint();
	ETPSUnitLevel bestLevel = ETPSUnitLevel::ROOT;

	for (auto sp : SpawnPoints)
	{
		if (bestLevel < sp->AssignedUnit.UnitLevel) { continue;	}

		// Correct Squad/Unit found - return immediately
		if (FTPSUnitID::HashHierarchy(sp->AssignedUnit) == FTPSUnitID::HashHierarchy(unitId))
		{
			if (sp->PreferredSquadRoleToSpawn == role && sp->IsSpawnAvailable())
			{
				return sp;
			}
		}

		// Near Fit - keep scanning
		/*if (auto unitNumber = unitId.Hierarchy.Find(sp->AssignedUnit.UnitLevel))
		{
			if (*unitNumber == sp->AssignedUnit.UnitNumber
				&& sp->PreferredSquadRoleToSpawn == role 
				&& sp->IsSpawnAvailable())
			{
				bestSpawn = sp;
				bestLevel = sp->AssignedUnit.UnitLevel;				
			}
		}*/
	}

	return bestSpawn;
}
