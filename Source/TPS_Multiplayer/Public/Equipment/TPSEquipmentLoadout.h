// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSEquipmentDefinition.h"

#include "Equipment/TPSEquipableItem.h"
#include "Equipment/TPSEquipmentSlot.h"

#include "TPSEquipmentLoadout.generated.h"

UCLASS(BlueprintType, Const)
class TPS_MULTIPLAYER_API UTPSEquipmentLoadout : public UDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    TMap<TEnumAsByte<ETPSEquipmentSlot>, UTPSEquipmentDefinition*> EquipmentBySlot;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATPSEquipableItem> PrimaryWeapon;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATPSEquipableItem> SecondaryWeapon;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATPSEquipableItem> TertiaryWeapon;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATPSEquipableItem> LethalEquipment;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATPSEquipableItem> TacticalEquipment;
};
