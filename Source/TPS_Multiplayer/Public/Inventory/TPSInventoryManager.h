// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSInventoryItem.h"

#include "TPSInventoryManager.generated.h"

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSInventoryCollection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<UTPSInventoryItem*> items;
};

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSInventoryManager : public UActorComponent
{
    GENERATED_BODY()
public:
    UTPSInventoryManager() {};

protected:
    FTPSInventoryCollection Inventory;
};
