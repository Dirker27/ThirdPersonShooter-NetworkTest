// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TPSCharacterState.h"

#include "TPSPlayerController.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsDebugEnabled = false;

	UFUNCTION(Exec, Category="Debug.Overrides")
	void TPS_OverrideCurrentCharacterStateFromInt(int32 state);

	UFUNCTION(Exec, Category = "Debug.Overrides")
	void TPS_OverrideCurrentCharacterStateFromString(FString state);

	UFUNCTION(Exec, Category="Debug.Visibility")
	void TPS_ShowDebugForLocalPlayer();

	UFUNCTION(BlueprintCallable)
	void BindConsoleCallbacks();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDebugInput();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterStateOverride(ETPSCharacterState state);

private:
	void OnDebugInputDelegate(IConsoleVariable* Var);
};
