// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Team/Types/TPSUnitID.h"

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
	FLinearColor BannerColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTPSUnitIdentifier* FlagshipUnit ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid LeadCharacterId;
};
