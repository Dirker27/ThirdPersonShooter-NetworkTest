// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipableItem.h"
#include "Equipment/TPSEquipmentSlot.h"

#include "TPSEquipmentLoadout.generated.h"

/**
 * State object for a character's weapons, equipment, and gear.
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSEquipmentLoadout : public UDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSEquipmentSlot>, TSubclassOf<ATPSEquipableItem>> EquipmentBySlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSGearSlot>, TSubclassOf<ATPSEquipableItem>> GearBySlot;
};
