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
	TArray<TObjectPtr<ATPSSPawnPoint>> SpawnPoints;

public:
	UFUNCTION(BlueprintCallable)
	TArray<ATPSSPawnPoint*> GetSpawnPoints() const { return SpawnPoints; }

	UFUNCTION(BlueprintCallable)
	void AddSpawnPointToPool(ATPSSPawnPoint* spawn);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSSPawnPoint* FindFirstAvailableSpawnPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSSPawnPoint* FindBestSpawnPointForSquadRole(const ETPSSquadRole role);
};