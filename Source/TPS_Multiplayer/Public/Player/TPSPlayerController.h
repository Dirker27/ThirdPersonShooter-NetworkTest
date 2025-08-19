// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

#include "Character/TPSCharacterBehaviorState.h"
#include "TPSControllerConfiguration.h"
#include "Character/TPSCharacter.h"
#include "GAS/GASAbilitySet.h"

#include "TPSPlayerController.generated.h"

class ATPSPawn;

UCLASS()
class TPS_MULTIPLAYER_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//~ ==================================================================== ~//
//  COMPONENTS
//~ ==================================================================== ~//

protected:
	virtual void SetupInputComponent() override;
	virtual void OnRep_PlayerState() override;

//~ ==================================================================== ~//
//  ATTRIBUTES
//~ ==================================================================== ~//
public:

	////////////////////////////////////////////////////////
	// TPSGameState

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<ATPSCharacter> PossessedCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<ATPSPawn> PossessedPawn;

	// Disables Camera->Character Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFreeCam;

//~ ==================================================================== ~//
//  BEHAVIOR OPERATIONS
//~ ==================================================================== ~//
public:
	FVector GetCameraTargetLocation() const;

	void NotifyPawnDeath();

	UFUNCTION(Server, Reliable, Category = "Pawn|Respawn")
	void RequestRespawn();

	UFUNCTION(Server, Reliable, Category = "Pawn|Possess")
	void PossessNearestPlayablePawn();

	UFUNCTION(Server, Reliable, Category = "Pawn|Possess")
	void UnPossessCurrentPawn();


	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;



//~ ==================================================================== ~//
//  CONSOLE COMMANDS (Developer-only API)
//~ ==================================================================== ~//
public:

	////////////////////////////////////////////////////////
	// Override Character TPSGameState

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
//  LOCAL PLAYER CONFIGURATION
//  - Should Show Debug
//  - Overridden States
//~ ==================================================================== ~//
public:
	// Show DEBUG view to local player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDebugEnabled = false;

	UFUNCTION(BlueprintCallable)
	void BindConsoleCallbacks();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDebugInput();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterStateOverride(ETPSCharacterBehaviorState state);

	UFUNCTION(BlueprintCallable)
	static FTPSControllerConfiguration GetControllerConfiguration() {
		return *configuration;
	}
	static void UpdateControllerConfiguration(FTPSControllerConfiguration* config);

private:
	static FTPSControllerConfiguration* configuration;
};
