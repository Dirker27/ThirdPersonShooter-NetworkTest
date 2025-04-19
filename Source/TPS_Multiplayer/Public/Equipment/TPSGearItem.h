// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipableItem.h"
#include "Equipment/TPSGearSlot.h"

#include "TPSGearItem.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSGearItem : public ATPSEquipableItem
{
	GENERATED_BODY()

public:
	ATPSGearItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EquipableItem|Configuration")
	TEnumAsByte<ETPSGearSlot> GearSlot;

};
