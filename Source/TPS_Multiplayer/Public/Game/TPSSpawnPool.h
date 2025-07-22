// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Game/TPSSpawnPoint.h"

#include "TPSSpawnPool.generated.h"


UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSSpawnPool : public UObject
{
	GENERATED_BODY()

public:
	UTPSSpawnPool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ATPSSpawnPoint>> SpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<int, TObjectPtr<ATPSSpawnPoint>> SpawnPointsById;

public:
	UFUNCTION(BlueprintCallable)
	TArray<ATPSSpawnPoint*> GetSpawnPoints() const { return SpawnPoints; }

	UFUNCTION(BlueprintCallable)
	void AddSpawnPointToPool(ATPSSpawnPoint* spawn);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSSpawnPoint* FindFirstAvailableSpawnPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSSpawnPoint* FindBestSpawnPointForSquadRole(const ETPSSquadRole role);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSSpawnPoint* FindBestSpawnPointForUnitAndSquadRole(const FTPSUnitID unitId, const ETPSSquadRole role);

};
