// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

#include "Character/TPSCharacterState.h"
#include "TPSControllerConfiguration.h"
#include "GAS/GASAbilitySet.h"

#include "TPSPlayerController.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();

	// Show DEBUG view to local player.
	UPROPERTY(BlueprintReadOnly)
	bool IsDebugEnabled = false;

	virtual void BeginPlay() override;

//~ ==================================================================== ~//
//  COMPONENTS
//~ ==================================================================== ~//

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* ControllerCamera;


	virtual void SetupInputComponent() override;

	virtual void OnRep_PlayerState() override;

	void BindInputToPlayerStateASC();


//~ ==================================================================== ~//
//  BEHAVIOR
//~ ==================================================================== ~//
public:
	void NotifyPawnDeath();

	UFUNCTION(Server, Reliable, Category = "Pawn|Respawn")
	void RequestRespawn();

	UFUNCTION(Server, Reliable, Category = "Pawn|Possess")
	void PossessNearestPlayablePawn();

	UFUNCTION(Server, Reliable, Category = "Pawn|Possess")
	void UnPossessCurrentPawn();


//~ ==================================================================== ~//
//  CONSOLE COMMANDS (Developer-only API)
//~ ==================================================================== ~//
public:

	////////////////////////////////////////////////////////
	// Override Character State

	UFUNCTION(Exec, Category="Debug|Overrides")
	void TPS_OverrideCurrentCharacterStateFromInt(int32 state);
	//
	UFUNCTION(Exec, Category = "Debug|Overrides")
	void TPS_OverrideCurrentCharacterStateFromString(FString state);
	//
	UFUNCTION(Exec, Category = "Debug|Overrides")
	void Kill();
	//
	UFUNCTION(Exec, Category = "Debug|Overrides")
	void Injure();
	//
	UFUNCTION(Exec, Category = "Debug|Overrides")
	void EnterCombat();
	//
	UFUNCTION(Exec, Category = "Debug|Overrides")
	void ExitCombat();


	////////////////////////////////////////////////////////
	// Override Controller Configurations

	UFUNCTION(Exec, Category="Config|Controls")
	void TPS_ToggleCrouchForLocalPlayer();
	UFUNCTION(Exec, Category = "Config|Controls")
	void ToggleCrouch();


	////////////////////////////////////////////////////////
	// Visual Debugging

	UFUNCTION(Exec, Category="Debug|Visibility")
	void TPS_ToggleDebugForLocalPlayer();
	UFUNCTION(Exec, Category = "Debug|Visibility")
	void DebugLocal();


	////////////////////////////////////////////////////////
	// Character / Pawn Controls

	UFUNCTION(Exec, Category = "Pawn|Respawn")
	void Respawn();

	UFUNCTION(Exec, Category = "Pawn|Possess")
	void PossessPawn();

	UFUNCTION(Exec, Category = "Pawn|Possess")
	void UnPossessPawn();


//~ ==================================================================== ~//
//  CONFIGURATION
//~ ==================================================================== ~//
public:

	UFUNCTION(BlueprintCallable)
	void BindConsoleCallbacks();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDebugInput();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterStateOverride(ETPSCharacterState state);

	UFUNCTION(BlueprintCallable)
	static FTPSControllerConfiguration GetControllerConfiguration() {
		return *configuration;
	}
	static void UpdateControllerConfiguration(FTPSControllerConfiguration* config);

private:
	static FTPSControllerConfiguration* configuration;
};
