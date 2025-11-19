// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSGearItem.h"
#include "Equipment/TPSGearSlot.h"
#include "Weapon/TPSWeapon.h"
#include "Weapon/Types/TPSWeaponSlot.h"

#include "TPSEquipmentLoadout.generated.h"

/**
 * State object for a character's weapons, equipment, and gear.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSEquipmentLoadoutData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSWeaponSlot>, TSubclassOf<ATPSWeapon>> WeaponsBySlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSGearSlot>, TSubclassOf<ATPSGearItem>> GearBySlot;
};



/**
 * State object container for a character's equipment Loadout
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSEquipmentLoadout : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSEquipmentLoadoutData Loadout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSWeaponSlot>, TSubclassOf<ATPSWeapon>> WeaponsBySlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSGearSlot>, TSubclassOf<ATPSGearItem>> GearBySlot;
};
