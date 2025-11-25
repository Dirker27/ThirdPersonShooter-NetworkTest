// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Army/Unit/TPSUnitID.h"
#include "Engine/DataAsset.h"
#include "Faction/TPSFactionID.h"
#include "Unit/TPSUnitID.h"

#include "TPSArmyIdentity.generated.h"

/**
 * Characterizes a given army.
 *
 * Banner colors, tactical style, favorite pizza toppings, etc.
 */
UCLASS(BlueprintType)
class UTPSArmyIdentity : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Alias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSFactionID> FactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BannerColor = FLinearColor::Black;
};
