// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "TPSPlayerController.h"

#include "TPSGameMode.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	bool RequestRespawn();

	void PerformRespawn(ATPSPlayerController playerController);

	//- Console Commands --------------------------------------------=
	//
	UFUNCTION(Exec)
	void ShowDebugForAll();
};
