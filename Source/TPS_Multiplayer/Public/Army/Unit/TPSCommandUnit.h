// (C) ToasterCat Studios 2025

#pragma once

#include "TPSUnitIdentity.h"
#include "TPSUnitSchema.h"
#include "TPSCommandGroup.h"

#include "Character/TPSCharacterInstance.h"

#include "TPSCommandUnit.generated.h"

/**
 * An instance of command-able units with a designated leader.
 *   ie: "Squad", "Platoon", "Fireteam", etc
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandUnit : public UTPSCommandGroup
{
	friend class UTPSArmyInstance;
	friend class UTPSCharacterInstance;
	friend class UTPSTeamInstance;

	GENERATED_BODY()

public:
	UTPSCommandUnit();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitSchemaData Schema;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FTPSUnitIdentityData Identity;



protected:
	////////////////////////////////////////////////////////
	// Assignment Info (Team/Army/Unit)
	//
	// TODO: Elevate to Assignable Interfaces [PC-237] [PC-236] [PC-257]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSArmyInstance> AssignedArmy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSTeamInstance> AssignedTeam;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSArmyInstance* GetAssignedArmy() { return AssignedArmy.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeamInstance* GetAssignedTeam() { return AssignedTeam.Get(); }

	// Assigned Team ID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ETPSTeamID GetAssignedTeamID() const;

	UFUNCTION(BlueprintCallable)
	void AssignToArmy(UTPSArmyInstance* army);


	////////////////////////////////////////////////////////
	// Leadership Info (Team/Army/Unit)

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSCharacterInstance> Leader;


public:
	UFUNCTION(BlueprintCallable)
	void SetLeader(UTPSCharacterInstance* member);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetLeader() const;
};
