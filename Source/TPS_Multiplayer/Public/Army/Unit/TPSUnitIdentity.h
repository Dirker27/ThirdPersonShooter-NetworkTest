// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSUnitIdentity.generated.h"


USTRUCT(BlueprintType)
struct FTPSUnitIdentityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Alias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BannerColor = FLinearColor::Black;
};


/**
 * Characterizes a given unit.
 *
 * Banner colors, tactical style, favorite pizza toppings, etc.
 */
UCLASS(BlueprintType)
class UTPSUnitIdentity : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitIdentityData Identity;
};
