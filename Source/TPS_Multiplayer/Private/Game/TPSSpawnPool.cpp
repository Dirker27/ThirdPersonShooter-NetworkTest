// (C) ToasterCat Studios 2025

#include "Game/TPSSpawnPool.h"

UTPSSpawnPool::UTPSSpawnPool()
{
	
}


void UTPSSpawnPool::AddSpawnPointToPool(ATPSSPawnPoint* spawn)
{
	SpawnPoints.Add(spawn);
}

ATPSSPawnPoint* UTPSSpawnPool::FindFirstAvailableSpawnPoint()
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

ATPSSPawnPoint* UTPSSpawnPool::FindBestSpawnPointForSquadRole(const ETPSSquadRole role)
{
	for (auto sp : SpawnPoints)
	{
		if (sp->PreferredSquadRoleToSpawn == role && sp->IsSpawnAvailable())
		{
			return sp;
		}
	}
	return nullptr;
}


