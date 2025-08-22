// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSCharacterInstance.h"

#include "TPSReport.generated.h"

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSEliminationReport
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTPSCharacterInstance* VictimCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTPSCharacterInstance* KillerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName KillMethod = FName();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Timestamp = 0;

	FTPSEliminationReport() { }

	FTPSEliminationReport(UTPSCharacterInstance* victim, UTPSCharacterInstance* killer, FName method, float timestamp)
	{
		VictimCharacter = victim;
		KillerCharacter = killer;
		KillMethod = method;
		Timestamp = timestamp;
	}
};



UCLASS()
class TPS_MULTIPLAYER_API UTPSKillInfo : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTPSEliminationReport Report;
};