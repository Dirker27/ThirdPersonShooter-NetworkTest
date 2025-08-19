// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSGearItem.h"
#include "Equipment/TPSGearSlot.h"
#include "Weapon/TPSWeapon.h"
#include "Weapon/TPSWeaponSlot.h"

#include "TPSEquipmentLoadout.generated.h"

/**
 * TPSGameState object for a character's weapons, equipment, and gear.
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSEquipmentLoadout : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSWeaponSlot>, TSubclassOf<ATPSWeapon>> WeaponsBySlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSGearSlot>, TSubclassOf<ATPSGearItem>> GearBySlot;
};
