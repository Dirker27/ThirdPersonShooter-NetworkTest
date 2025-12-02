// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSConstants.generated.h"

static const FLinearColor TEXT_COLOR_ABILITY_LOG = FLinearColor::Yellow;

static const TArray<FString> _NATO_ALPHABET = {
	"Alpha",
	"Bravo",
	"Charlie",
	"Delta",
	"Echo",
	"Foxtrot",
	"Golf",
	"Hotel",
	"India",
	"Juliett",
	"Kilo",
	"Lima",
	"Mike",
	"November",
	"Omega",
	"Papa",
	"Quebec",
	"Romeo",
	"Sierra",
	"Tango",
	"Uniform",
	"Victor",
	"Whiskey",
	"X-Ray",
	"Yankee",
	"Zulu",
};


UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSConstants : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor TEXT_COLOR_SERVER_LOG = FLinearColor::Black;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor TEXT_COLOR_CLIENT_LOG = FLinearColor::Black;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor TEXT_COLOR_COMBAT_LOG = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TEXT_DISPLAY_DURATION_COMBAT_LOG = 5.f;
};
