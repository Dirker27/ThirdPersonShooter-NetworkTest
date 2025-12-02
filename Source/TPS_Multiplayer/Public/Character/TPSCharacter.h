// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "TPSCharacterConfiguration.h"
#include "TPSCharacterID.h"
#include "TPSCharacterIdentity.h"
#include "TPSCharacterInventory.h"

#include "Character/Types/TPSCharacterLocomotionState.h"
#include "Character/Types/TPSCharacterBehaviorState.h"
#include "Character/Types/TPSCharacterStance.h"
#include "Equipment/TPSEquipmentManager.h"
#include "GAS/TPSAbilitySystemComponent.h"
#include "GAS/Attributes/CharacterHealthAttributeSet.h"
#include "GAS/Attributes/StandardAttributeSet.h"
#include "GAS/Attributes/WeaponAttributeSet.h"
#include "Team/TPSTeamID.h"
#include "Weapon/TPSWeapon.h"

#include "TPSCharacter.generated.h"

class UTPSCharacterInstance;
class UTPSCommandUnit;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterAttributeUpdate);

/**
 * Defines an arbitrary Character in our game's universe.
 *
 *   - Identity: A character's persona (name, alignment, assignment)
 *   - State: Life experience (XP, stats, and history)
 *   - Configuration: Attributes and characteristics (Strength, accuracy)
 */
UCLASS()
class TPS_MULTIPLAYER_API ATPSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();
	~ATPSCharacter();
	
// UE Implementables
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


//~ ======================================================================== ~//
//  COMPONENTS
//~ ======================================================================== ~//
public:
	//////////////////////////////////////////////////////
	// Display Widgets

	// Broadcast Delegate
	UPROPERTY(BlueprintAssignable)
	FCharacterAttributeUpdate CharacterActorUpdate;
private:
	// Trigger a broadcast to all listening display widgets to perform an Update cycle.
	//   Should set to 'true' whenever states or attributes are changed.
	bool ShouldNotify = false;


	//////////////////////////////////////////////////////
	// Inventory System
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UTPSEquipmentManager> EquipmentManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UTPSCharacterInventory> Inventory;



//~ ======================================================================== ~//
//  INSTANCE LINKING
//~ ======================================================================== ~//

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_CharacterInstance)
	TObjectPtr<UTPSCharacterInstance> CharacterInstance;

	UFUNCTION()
	void OnRep_CharacterInstance();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetCharacterInstance() { return CharacterInstance; }

	UFUNCTION(BlueprintCallable)
	void BindToCharacterInstance(UTPSCharacterInstance* instance);

	// (Re)Initialize the Character based off its bound Instance
	UFUNCTION(BlueprintCallable)
	void Initialize();
	// (Re)Initialize the Character based off its bound Instance - Blueprint Extension
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInitialize();


//~ ======================================================================== ~//
//  STATE
//~ ======================================================================== ~//
public:

	//////////////////////////////////////////////////////
	// Identity

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTPSCharacterID GetCharacterID() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTPSOperatorIdentity GetIdentity() const;

	//////////////////////////////////////////////////////
	// Configuration

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTPSCharacterConfigurationData GetConfiguration() const;

	// Can Be Possessed by a Player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPSCharacter|Configuration")
	bool CanBePossessedByPlayer = false;

	// Can Death be triggered by loss of Health?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Configuration")
	bool CanDie = false;

	// Character Attributes Configuration
	//   Can be overridden by PlayerState on Possession.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TPSCharacter|Configuration", Replicated)
	TObjectPtr<UTPSCharacterConfiguration> Configuration;



	//////////////////////////////////////////////////////
	// Assignment Info

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ETPSTeamID GetAssignedTeamID() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeamInstance* GetAssignedTeam() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandUnit* GetAssignedUnit() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSArmyInstance* GetAssignedArmy() const;



	//////////////////////////////////////////////////////
	// Attributes
	//   (sync'd from GAS attributes where applicable)
	//   TODO: Migrate to "CharacterAttributes" Object

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health")
	float MaxHealth;

	// Armor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Armor")
	float CurrentArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Armor")
	float MaxArmor;

	// Movement Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Movement")
	float MovementSpeedModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Movement")
	float CurrentMaxWalkSpeed;

	// Current Accuracy Tolerance (Synthetic)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetCurrentAccuracyTolerance() const;



	//////////////////////////////////////////////////////
	// Volatile State
	//
	// TODO: Migrate to Instance(?) and convert to synthetic getters

	// Behavior State (Casual, Combat, Incapacitated, etc)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State", Replicated)
	TEnumAsByte<ETPSCharacterBehaviorState> CurrentBehaviorState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State")
	TEnumAsByte<ETPSCharacterBehaviorState> PreviousBehaviorState;
	//
	// Locomotion State (Crouching, Sprinting, Ragdoll, etc)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State", Replicated)
	TEnumAsByte<ETPSCharacterLocomotionState> CurrentLocomotionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State")
	TEnumAsByte<ETPSCharacterLocomotionState> PreviousLocomotionState;
	//
	// Stance (Right/Left-Handed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State", Replicated)
	TEnumAsByte<ETPSCharacterStance> Stance;
	//
	// Has death been triggered?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool HasDeathTriggered;
	//
	// IsInitialized (Synthetic)
	//   True if bound to CharacterInstance.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInitialized() const;
	//
	// IsAlive (Synthetic)
	//   True if Character is not Incapacitated.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive() const;
	//
	// IsCrouching (Synthetic)
	//   True if Crouching OR Prone.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCrouching() const;
	//
	// IsIdle (Synthetic)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsIdle() const;
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float IdleSeconds;


	//////////////////////////////////////////////////////
	// Targeting

	// Target Rotation - Derived from Target Location (Replicated to peer clients)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input|Targeting", Replicated)
	FRotator TargetLookRotation;
	//
	// Should Target Location drive Target Rotation?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input|Targeting")
	bool IsTargetingLocation;
	//
	// Target Location - Provided by LOCAL Controller (Not Replicated)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input|Targeting")
	FVector TargetLookLocation;
	// Current Look Location - Iterps to TargetLocation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input|Targeting")
	FVector CurrentLookLocation;
	// Interp rate for CurrentLookLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input|Targeting")
	float LookTargetInterpRate = 0.8f;


	////////////////////////////////////////////////////////
	// Equipment State

	// Current Equipped Weapon (Synthetic)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSWeapon* GetEquippedWeapon() const;

	////////////////////////////////////////////////////////
	// Input / Ability States

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsBoosting;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsCrouchInputReceived;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsAiming;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsFiring;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsEquipping;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsReloading;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsInteracting;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsTransitioning;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Input", Replicated)
	bool IsInMenu;

	//////////////////////////////////////////////////////
	// UI Visibility

	// Shows full diagnostic data to peer client/server
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool IsDebugEnabled = false;
	//
	// "Highlight" character to peer display (turn on shiny shader)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool IsFocused = false;
	//
	// Shows name/health data to peer client/server
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool IsUnitFrameEnabled = true;
	//
	// Shows simple debug data to peer client
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool IsDebugFrameEnabled = true;



//~ ======================================================================== ~//
//  Character Business Logic
//~ ======================================================================== ~//

private:
	// Called every frame.
	//  Set state-driven values in subcomponents.
	void SyncComponentsFromState();

public:

	////////////////////////////////////////////////////////
	// State Modifiers

	// CharacterState
	UFUNCTION(BlueprintCallable)
	void ApplyBehaviorState(const ETPSCharacterBehaviorState CharacterState);
	UFUNCTION(BlueprintCallable)
	void RevertCharacterState();
	//
	// LocomotionState
	UFUNCTION(BlueprintCallable)
	void ApplyLocomotionState(const ETPSCharacterLocomotionState LocomotionState);
	UFUNCTION(BlueprintCallable)
	void RevertLocomotionState();

	UFUNCTION(BlueprintCallable)
	void InterruptIdle();

	UFUNCTION(BlueprintCallable)
	void SetTargetLocation(FVector targetLocation);


	////////////////////////////////////////////////////////
	// Business Logic

	// Determine what state we should be in.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ETPSCharacterLocomotionState EvaluateLocomotionStateForCurrentInput();
	//
	// Calculate Speed
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBaseSpeedForCharacterState(const ETPSCharacterBehaviorState CharacterState);
	//
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function"))
	float GetSpeedModifierForLocomotionState(const ETPSCharacterLocomotionState LocomotionState);
	//
	// Apply calculated MovementSpeed
	UFUNCTION(BlueprintCallable)
	float UpdateCharacterSpeedForCurrentState();
	//
	// Are there any actions ongoing that would be considered "Active"?
	//   (aiming, firing, equipping, etc)
	//   Determines whether a character is in full SPRINT vs simply boosting.
	UFUNCTION(BlueprintCallable)
	bool IsActionActive() const;


	// "Are we dead yet?"
	//    Evaluated in OnTick() to allow for alternative sources of death than strict damage.
	//    (Fell out of world, overly idle, etc)
	UFUNCTION(BlueprintCallable)
	bool IsDeathConditionMet();


	// Character Death [RPC]
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Die();
	//
	// Start the process of dying [Multicast]
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void StartDeath();
	// Finish Dying (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathStart();
	//
	// Finish the process of dying - Called by Blueprint
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void CompleteDeath();
	// Finish Dying (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathComplete();




//~ ======================================================================== ~//
//  CONTROLLER / PLAYER POSSESSION
//~ ======================================================================== ~//
protected:
	// Bind to AbilitySystem in PlayerState
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;

//~ ======================================================================== ~//
//  ABILITY SYSTEM
//~ ======================================================================== ~//
protected:
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UTPSAbilitySystemComponent* AbilitySystem{ nullptr };

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface


	////////////////////////////////////////////////////////
	// GAS Attributes

	// Movement, Mana, and Stamina
	UPROPERTY(VisibleAnywhere, Category = "Abilities|Attributes", Transient)
	UStandardAttributeSet* StandardAttributes{ nullptr };

	// Health, Armor, and Regen
	UPROPERTY(VisibleAnywhere, Category = "Abilities|Attributes", Transient)
	UCharacterHealthAttributeSet* CharacterHealthAttributes{ nullptr };

	// Damage, Accuracy, and Modifiers
	UPROPERTY(VisibleAnywhere, Category = "Abilities|Attributes", Transient)
	UWeaponAttributeSet* WeaponAttributes{ nullptr };

protected:
	// Sync's local variables from GAS attributes.
	//   ie: CharacterHealth.Health -> CurrentHealth
	// Called every frame.
	UFUNCTION(BlueprintCallable)
	void SyncAttributesFromGAS();

	void OnArmorAttributeChanged(const FOnAttributeChangeData&);
	void OnHealthAttributeChanged(const FOnAttributeChangeData&);
	void OnMovementAttributeChanged(const FOnAttributeChangeData&);



	////////////////////////////////////////////////////////
	// Initialization (Grant Abilities to System)
protected:
	void SetupAbilitySystem();

	////////////////////////////////////////////////////////
	// Input Routing
protected:
	// Bind Input->AbilitySystem
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


//~ ======================================================================== ~//
//  Blueprint Extensions
//~ ======================================================================== ~//
public:
	////////////////////////////////////////////////////////
	// Ability Extensions

	// Boost - Called from Gameplay Ability.
	UFUNCTION(BlueprintCallable)
	void StartBoost();
	// Trigger additional animations and SFX for Boost ability. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnBoostAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndBoost();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBoostAbilityEnd();
	//
	// Aim - Called from Gameplay Ability.
	UFUNCTION(BlueprintCallable)
	void StartAim();
	// Trigger animations and SFX for Aim ability. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndAim();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimAbilityEnd();
	//
	// Fire Weapon - Called from Gameplay Ability.
	UFUNCTION(BlueprintCallable)
	void StartFireWeapon();
	// Trigger animations and SFX for Fire Weapon action started. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnFireWeaponAbilityStart();
	// Called when fire is actually performed to trigger animations
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFirePerformed();
	//
	// Stop Firing Weapon - Called from Gameplay Ability.
	UFUNCTION(BlueprintCallable)
	void EndFireWeapon();
	// Trigger animations and SFX for ending Fire Weapon. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnFireWeaponAbilityEnd();
	//
	// EquipWeapon ("Ready") - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void StartEquipWeapon();
	// Trigger additional animations and SFX for Equipping Weapon. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipWeaponAbilityStart();
	//
	// EndEquipWeapon ("FinishReady") - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void EndEquipWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipWeaponAbilityEnd();
	//
	// UnEquipWeapon ("UnReady") - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void StartUnEquipWeapon();
	// Trigger additional animations and SFX for UnEquipping Weapon. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnEquipWeaponAbilityStart();
	//
	// UnEquipWeapon ("Finish UnReady") - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void EndUnEquipWeapon();
	// Trigger additional animations and SFX for UnEquipping Weapon. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnEquipWeaponAbilityEnd();
	//
	// Reload Weapon - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void StartReloadWeapon();
	// Trigger additional animations and SFX for Reloading weapon. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnReloadWeaponAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndReloadWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReloadWeaponAbilityEnd();
	//
	// Interact Start - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void StartInteract();
	// Trigger additional animations and SFX for Interact Start. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractAbilityStart();
	//
	// Interact End - Called from Gameplay Ability
	UFUNCTION(BlueprintCallable)
	void EndInteract();
	// Trigger additional animations and SFX for Interact End. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractAbilityEnd();
	//
	// Switch Stance - Called from Gameplay Ability.
	UFUNCTION(BlueprintCallable)
	void StartSwitchStance();
	// Trigger additional animations and SFX for Boost ability. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnSwitchStanceAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndSwitchStance();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSwitchStanceAbilityEnd();
	//
	// Switch Stance - Called from Gameplay Ability.
	UFUNCTION(BlueprintCallable)
	void StartUseMenu();
	// Trigger additional animations and SFX for Boost ability. (Blueprint Extension)
	UFUNCTION(BlueprintImplementableEvent)
	void OnUseMenuAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndUseMenu();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUseMenuAbilityEnd();

	////////////////////////////////////////////////////////
	// Behavior Overrides

	UFUNCTION(BlueprintImplementableEvent)
	void OnFellOutOfWorld();


	////////////////////////////////////////////////////////
	// Type Transforms

	// CharacterState
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString CharacterStateToFString(ETPSCharacterBehaviorState s) {
		return FString(ETPSCharacterStateToString(s));
	};
	//
	// LocomotionState
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString LocomotionStateToFString(ETPSCharacterLocomotionState s) {
		return FString(ETPSLocomotionStateToString(s));
	}

//~ ======================================================================== ~//
//  MISC BEHAVIOR OVERRIDES
//~ ======================================================================== ~//

public:
	// Use Actor's Eyes at Mesh Location for detection and docking sweeps.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Schema")
	FName EyeSocketName;
	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	// Activate Ragdoll and die when outside world boundaries. (impl'd in BP)
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;


	// Apply damage via Ability System (if present)
	//virtual float TakeDamage(float damage, struct FDamageEvent const& event, AController* instigator, AActor* causer) override;
};