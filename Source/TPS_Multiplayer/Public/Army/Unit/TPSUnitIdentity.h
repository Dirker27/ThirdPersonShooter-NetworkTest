// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Army/Unit/TPSUnitID.h"

#include "TPSUnitIdentity.generated.h"

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
	FString Alias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BannerColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTPSUnitIdentifier* FlagshipUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid LeadCharacterId;
};
