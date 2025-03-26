// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Inventory/TPSEquipmentSlot.h"
#include "Weapon/TPSWeapon.h"

#include "TPSLoadout.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API UTPSLoadout : public UDataAsset
{
    GENERATED_BODY()

public:
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
