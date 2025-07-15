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


ATPSSpawnPoint* UTPSSpawnPool::FindBestSpawnPointForUnitAndRole(const FTPSUnitID uid, const ETPSSquadRole role)
{
	Do this from the GM instead - we only should provide FIRST and TYPED from here.

	return FindBestSpawnPointForSquadRole(role);
}

