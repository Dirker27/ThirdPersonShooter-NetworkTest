// (C) ToasterCat Studios 2025

#pragma once

#include "TPSUnitIdentity.h"
#include "Army/Unit/TPSUnitSchema.h"
#include "Army/Unit/TPSCommandGroup.h"
#include "Character/TPSCharacterInstance.h"

#include "TPSCommandUnit.generated.h"

/**
 * An instance of command-able units with a designated leader.
 *   ie: "Squad", "Platoon", "Fireteam", etc
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandUnit : public UTPSCommandGroup
{
	GENERATED_BODY()

public:
	UTPSCommandUnit();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitSchemaData Schema;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSUnitIdentity> Identity;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSCharacterInstance> Leader;

public:
	UFUNCTION(BlueprintCallable)
	void SetLeader(UTPSCharacterInstance* member);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetLeader() const;
};
