// (C) ToasterCat Studios 2025

#pragma once

#include "Game/TPSSpawnPool.h"
#include "Team/TPSCommandUnitConfiguration.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSCommandUnitConfiguration Configuration;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UTPSCharacterInstance> Leader;

public:
	UFUNCTION(BlueprintCallable)
	void SetLeader(UTPSCharacterInstance* member);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetLeader() const;
};
