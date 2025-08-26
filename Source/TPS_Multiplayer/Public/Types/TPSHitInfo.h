// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSHitInfo.generated.h"

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSHitInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FHitResult HitInfo;

	// The Character/Pawn that sent the hit
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Instigator;

	// The Player/AI Controller responsible
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AController> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Timestamp;
};



UCLASS()
class TPS_MULTIPLAYER_API UDeathInfo : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTPSHitInfo TpsHitInfo;



	// TODO: Timestamp / Expiry protocol

};