#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSInventoryItem.generated.h"

USTRUCT()
struct TPS_MULTIPLAYER_API FTPSInventoryItemIdentifier
{
	GENERATED_BODY()

	FName Id;
};

UCLASS()
class TPS_MULTIPLAYER_API UTPSInventoryItem : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Identity")
	FTPSInventoryItemIdentifier Identifier;

	UPROPERTY(EditAnywhere, Category = "Identity")
	FName Name;

	UPROPERTY(EditAnywhere, Category = "Identity")
	TObjectPtr<USkeletalMesh> Avatar;

	UPROPERTY(EditAnywhere, Category = "Quantity")
	bool IsStackable;

	UPROPERTY(EditAnywhere, Category = "Quantity")
	int Quantity;
};