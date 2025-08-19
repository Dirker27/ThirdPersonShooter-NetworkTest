// (C) ToasterCat Studios 2025

#pragma once

#include "Team/TPSCommandUnitConfiguration.h"
#include "Team/TPSCommandGroup.h"

#include "TPSCommandStructure.generated.h"

/**
 * A generic unit of command-able units with a designated leader.
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandStructure : public UTPSCommandGroup
{
	GENERATED_BODY()

public:
	UTPSCommandStructure();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSCommandUnitConfiguration Configuration;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSCharacterInstance> Leader;

public:
	UFUNCTION(BlueprintCallable)
	void SetLeader(UTPSCharacterInstance* member);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetLeader() const;
};
