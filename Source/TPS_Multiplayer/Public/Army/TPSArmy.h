// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSArmyID.h"

#include "Army/Unit/TPSCommandUnit.h"

#include "TPSArmy.generated.h"


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmyUpdate);

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSArmyInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSArmyInstance() {};

    // Broadcast Delegate - Team/Character/Player Roster Updated
    UPROPERTY(BlueprintAssignable)
    FArmyUpdate ArmyUpdate;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;

    //~ ==================================================================== ~//
    //  ATTRIBUTES
    //~ ==================================================================== ~//
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
    TEnumAsByte<ETPSTeamID> TeamID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
    FTPSArmyID ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TObjectPtr<UTPSCommandUnit> RootUnit;
};
