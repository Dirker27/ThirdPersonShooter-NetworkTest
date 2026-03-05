// (C) ToasterCat Studios 2026

#pragma once

#include "CoreMinimal.h"

#include "TPSTargetInfo.generated.h"

/**
 * Represents an abstract targeted location / actor in the world.
 *
 * "TargetLocation" is a reserved term in UE - Actor.GetTargetLocation() is used for projectile calculation.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSTargetInfo
{
	GENERATED_BODY()

	// The Target Location
	//    alt names: "Terminus", "Mark", "Point"
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Location = FVector::Zero();

	// An actor that may/may not be targeted. May be nullptr.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* Actor = nullptr;
};
