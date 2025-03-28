// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Inventory/TPSInventoryItem.h"

#include "TPSCharacterInventory.generated.h"

// TODO: Deprecate?
UCLASS()
class TPS_MULTIPLAYER_API UTPSCharacterInventory : public UActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TArray<UTPSInventoryItem*> InventoryItems;
};