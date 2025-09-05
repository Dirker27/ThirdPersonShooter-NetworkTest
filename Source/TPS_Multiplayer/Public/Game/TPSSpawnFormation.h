// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "World/TPSSpawnPoint.h"

#include "TPSSpawnFormation.generated.h"


UCLASS()
class TPS_MULTIPLAYER_API ATPSSpawnFormation : public AActor
{
	GENERATED_BODY()

public:
	ATPSSpawnFormation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitID AssignedUnit;

private:
	UFUNCTION(BlueprintCallable)
	void IndexChildren();
};
