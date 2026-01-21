// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TPSPlayerState.h"
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


/**
 * A Player Controller used to drive Pawns and Characters.
 *
 * Responsible for binding a player's HUD and Controls to their currently possessed player.
 *
 * Will exist on SERVER and player's CLIENT (only the owning player)
 * 
 * Binds to:
 *  - Owner's PlayerState
 *  - Active Character / Pawn
 */
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
//  GAMEPLAY BEHAVIOR
//~ ==================================================================== ~//

	////////////////////////////////////////////////////////
	// Player Spawn
public:
	virtual bool CanRestartPlayer() override;


	////////////////////////////////////////////////////////
	// Targeting
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCameraTargetLocation() const;

	void NotifyPawnDeath();



	////////////////////////////////////////////////////////
	// UI States & Menu Selection
	//
	// TODO: [PC-254] Design UI Remote Activation System

	UFUNCTION(BlueprintCallable)
	void SetControllerState(const ETPSControllerState state);


//~ ==================================================================== ~//
//  POSSESSION && INSTANCE BINDINGS
//~ ==================================================================== ~//
protected:

	// Fires on SERVER
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	// Fires on CLIENT
	virtual void OnRep_Pawn() override;
	
	// Called by both Server and Client when Pawn is changed to perform
	//   possession logic and Bind/UnBind instances consistently.
	// Mitigates Possess<->UnPossess race condition.
	void OnPawnPossessionChanged();



	////////////////////////////////////////////////////////
	// Bind to PlayerState

	UFUNCTION(BlueprintCallable)
	void BindControllerToPlayer(ATPSPlayerState* newPlayer);
	// Race-safe listener for when this Controller is bound to its Player.
	//   Set initialization values and bindings here.
	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerBoundToPlayer(ATPSPlayerState* newPlayer);


	////////////////////////////////////////////////////////
	// Bind to Possessed Characters

	// Character that Controller has been Bound to.
	//   Used as a lingering reference to un-bind dependencies
	//   and callbacks from Character after un-possessing.
	//
	// NOT GUARANTEED to be the same as PossessedCharacter()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<ATPSCharacter> BoundCharacter = nullptr;

	UFUNCTION(BlueprintCallable)
	void BindControllerToCharacter(ATPSCharacter* newCharacter);
	UFUNCTION(BlueprintImplementableEvent)
	// Race-safe listener for when this Controller is bound to a new Character.
	//   Set possession values and bindings here.
	void OnControllerBoundToCharacter(ATPSCharacter* newCharacter);
	UFUNCTION(BlueprintCallable)
	// Race-safe listener for when this Controller is released from an old Character.
	//   Release bindings and reset possession values here.
	void UnBindControllerFromCharacter(ATPSCharacter* oldCharacter);
	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerUnBoundFromCharacter(ATPSCharacter* oldCharacter);


	////////////////////////////////////////////////////////
	// Bind to Possessed Pawns

	// Pawn that Controller has been Bound to.
	//   Used as a lingering reference to un-bind dependencies
	//   and callbacks from Pawn after un-possessing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<ATPSPawn> BoundPawn = nullptr;

	UFUNCTION(BlueprintCallable)
	void BindControllerToPawn(ATPSPawn* newPawn);
	// Race-safe listener for when this Controller is bound to a new Pawn.
	//   Set possession values and bindings here.
	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerBoundToPawn(ATPSPawn* newPawn);
	UFUNCTION(BlueprintCallable)
	// Race-safe listener for when this Controller is released from an old Pawn.
	//   Release bindings and reset possession values here.
	void UnBindControllerFromPawn(ATPSPawn* oldPawn);
	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerUnBoundFromPawn(ATPSPawn* oldPawn);



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
