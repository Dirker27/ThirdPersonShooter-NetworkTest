// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipableItem.h"
#include "Equipment/TPSEquipmentSlot.h"

#include "TPSLoadout.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API UTPSLoadout : public UDataAsset
{
    GENERATED_BODY()

public:

    // TODO: Slot-Item Map

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
