// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamID.generated.h"

static const int TEAM_SLOT_SIZE = 5;

UENUM(BlueprintType)
enum ETPSTeamID : int
{
	// TODO: Wrap with FTPSTeamID

	UNAFFILIATED = 0 UMETA(DisplayName = "UNAFFILIATED"), // UNAFFILIATED | Lone Wolves

	RED_TEAM = 1,
	BLUE_TEAM = 2,
	GREEN_TEAM = 3,
	GOLD_TEAM = 4
};

static FColor TPSTeamIdToColor(ETPSTeamID id)
{
	switch (id)
	{
	case ETPSTeamID::RED_TEAM:
		return FColor(255, 25, 25);
	case ETPSTeamID::BLUE_TEAM:
		return FColor(0, 255, 255);
	case ETPSTeamID::GREEN_TEAM:
		return FColor(24, 255, 96);
	case ETPSTeamID::GOLD_TEAM:
		return FColor::Orange;
	default:
		return FColor::White;
	}
}


static FString TPSTeamIdToString(ETPSTeamID id)
{
	switch (id)
	{
	case ETPSTeamID::RED_TEAM:
		return "RED_TEAM";
	case ETPSTeamID::BLUE_TEAM:
		return "BLUE_TEAM";
	case ETPSTeamID::GREEN_TEAM:
		return "GREEN_TEAM";
	case ETPSTeamID::GOLD_TEAM:
		return "GOLD_TEAM";
	default:
		return "[TEAM]";
	}
}



UCLASS(BlueprintType)
class UTPSTeamIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSTeamID> TeamID;
};
