// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"

#include "TPSControllerConfiguration.generated.h"

// "Controller" Schema?
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSControllerConfiguration
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	int LocalCharacterDebugMode = 0;

	UPROPERTY(BlueprintReadOnly)
	bool GodModeEnabled = false;

	UPROPERTY(BlueprintReadOnly)
	bool crouchToggle = true;
};
