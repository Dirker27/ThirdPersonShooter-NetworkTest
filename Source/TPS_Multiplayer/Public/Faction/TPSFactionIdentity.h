// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TPSFactionIdentity.generated.h"

/**
 * Characterizes a given Faction.
 *
 * Banner colors, tactical style, favorite pizza toppings, etc.
 */
UCLASS(BlueprintType)
class UTPSFactionIdentity : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FullName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ShortName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Abbreviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BannerColor = FLinearColor::Black;
};
