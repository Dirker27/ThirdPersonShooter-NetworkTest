// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

//#include "TPSSpawnPool.h"

#include "Army/Unit/TPSUnitHierarchy.h"
#include "Team/TPSTeamID.h"

#include "World/Spawn/TPSSpawnPoint.h"

#include "TPSSpawnFormation.generated.h"


UCLASS()
class TPS_MULTIPLAYER_API ATPSSpawnFormation : public AActor
{
	GENERATED_BODY()

public:
	ATPSSpawnFormation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSUnitLevel> UnitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSTeamID> AssignedTeamID;

	// Filters Squads/Fireteams that can use this Spawn Point
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitHierarchy UnitHierarchyFilter;

	// Will override all other selection filters if set.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UTPSCommandUnit> AssignedUnit;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<UTPSSpawnPool> SpawnPool;


private:
	UFUNCTION(BlueprintCallable)
	void IndexChildren();
};
