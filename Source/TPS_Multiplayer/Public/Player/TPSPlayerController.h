// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayerState.h"
#include "GameFramework/PlayerController.h"

#include "Character/Types/TPSCharacterBehaviorState.h"
#include "Character/TPSCharacter.h"
#include "Pawn/TPSPawn.h"
#include "Player/TPSControllerConfiguration.h"

#include "TPSPlayerController.generated.h"

UENUM(BlueprintType)
enum ETPSControllerState : int
{
	Initializing,
	Active,
	Disconnected
};


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerStateUpdate);

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


	// Broadcast Delegate
	UPROPERTY(BlueprintAssignable)
	FControllerStateUpdate ControllerStateUpdate;

//~ ==================================================================== ~//
//  LIVE STATE
//~ ==================================================================== ~//
public:

	////////////////////////////////////////////////////////
	// State

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_ControllerState)
	TEnumAsByte<ETPSControllerState> ControllerState = Initializing;

	UFUNCTION()
	void OnRep_ControllerState();


	// Disables Camera->Character Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFreeCam;



	////////////////////////////////////////////////////////
	// Synthetic Getters

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSCharacter* PossessedCharacter() { return Cast<ATPSCharacter>(GetPawn()); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSPawn* PossessedPawn() { return Cast<ATPSPawn>(GetPawn()); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSPlayerState* BoundPlayer() { return GetPlayerState<ATPSPlayerState>(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInitialized() { return IsValid(BoundPlayer()); }


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:

	////////////////////////////////////////////////////////
	// Initialization & Lifecycle

	UFUNCTION(BlueprintCallable)
	void BindToPlayer(ATPSPlayerState* newPlayer);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBindToPlayer(ATPSPlayerState* newPlayer);

	UFUNCTION(BlueprintCallable)
	void BindToCharacter(ATPSCharacter* newCharacter);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBindToCharacter(ATPSCharacter* newCharacter);
	UFUNCTION(BlueprintCallable)
	void UnBindFromCharacter(ATPSCharacter* oldCharacter);
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnBindFromCharacter(ATPSCharacter* oldCharacter);

	UFUNCTION(BlueprintCallable)
	void BindToPawn(ATPSPawn* newPawn);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBindToPawn(ATPSPawn* newPawn);
	UFUNCTION(BlueprintCallable)
	void UnBindFromPawn(ATPSPawn* oldPawn);
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnBindFromPawn(ATPSPawn* oldPawn);


	////////////////////////////////////////////////////////
	// Possession

	// Fires on SERVER
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// Fires on CLIENT
	virtual void OnRep_Pawn() override;



	////////////////////////////////////////////////////////
	// Targeting Behavior

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCameraTargetLocation() const;

	void NotifyPawnDeath();




	////////////////////////////////////////////////////////
	// UI States & Menu Selection

	UFUNCTION(BlueprintCallable)
	void SetControllerState(const ETPSControllerState state);

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

	UFUNCTION(Server, Reliable, Category = "Pawn|Respawn")
	void RequestRespawn();

	UFUNCTION(Server, Reliable, Category = "Pawn|Possess")
	void PossessNearestPlayablePawn();

	UFUNCTION(Server, Reliable, Category = "Pawn|Possess")
	void UnPossessCurrentPawn();

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
