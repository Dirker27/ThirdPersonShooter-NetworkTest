// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSPlayerCameraManager.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};