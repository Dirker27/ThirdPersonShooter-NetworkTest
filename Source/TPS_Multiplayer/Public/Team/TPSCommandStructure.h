// (C) ToasterCat Studios 2025

#pragma once

#include "Game/TPSSpawnPool.h"
#include "Team/TPSCommandStructureConfiguration.h"
#include "Team/TPSCommandGroup.h"

#include "TPSCommandStructure.generated.h"

/**
 * A generic unit of command-able units with a designated leader.
 *
 * 
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandStructure : public UTPSCommandGroup
{
	GENERATED_BODY()

public:
	UTPSCommandStructure();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSCharacterInstance> Leader;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTPSCommandStructureConfiguration> Configuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSSpawnPool> SpawnPool;

public:
	UFUNCTION(BlueprintCallable)
	void GrantLeader(UTPSCharacterInstance* member);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetLeader() const;
};
