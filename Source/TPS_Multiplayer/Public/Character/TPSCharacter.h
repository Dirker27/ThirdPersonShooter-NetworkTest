// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "TPSCharacterConfiguration.h"
#include "TPSCharacterIdentity.h"
#include "TPSCharacterInventory.h"
#include "TPSCharacterRecord.h"

#include "Character/TPSCharacterLocomotionState.h"
#include "Character/TPSCharacterBehaviorState.h"
#include "Equipment/TPSEquipmentManager.h"
#include "GAS/TPSAbilitySystemComponent.h"
#include "GAS/Attributes/CharacterHealthAttributeSet.h"
#include "GAS/Attributes/StandardAttributeSet.h"
#include "GAS/Attributes/WeaponAttributeSet.h"
#include "Weapon/TPSWeapon.h"

#include "TPSCharacter.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateCharacterAttributeDisplay);

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
	FUpdateCharacterAttributeDisplay NotifyDisplayWidgets;
private:
	bool ShouldNotify = false;



//~ ======================================================================== ~//
//  STATE
//~ ======================================================================== ~//
public:

	//////////////////////////////////////////////////////
	// Identity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State")
	TObjectPtr<UTPSCharacterIdentity> Identity;
	
	// Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Identity")
	FString Name;

	//////////////////////////////////////////////////////
	// Configuration

	// Can Be Possessed by a Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Configuration")
	bool CanBePossessedByPlayer = false;

	// Character Attributes Configuration
	//   Can be overridden by PlayerState on Possession.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Configuration")
	TObjectPtr<UTPSCharacterConfiguration> Configuration;


	//////////////////////////////////////////////////////
	// Attributes
	//   (sync'd from GAS attributes where applicable)

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health", Replicated)
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health")
	float MaxHealth;

	// Armor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health", Replicated)
	float CurrentArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health")
	float MaxArmor;

	// Movement Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health", Replicated)
	float MovementSpeedModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Health")
	float CurrentMaxWalkSpeed;

	// Current Accuracy Tolerance (Synthetic)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetCurrentAccuracyTolerance() const;


	//////////////////////////////////////////////////////
	// Persistent State

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State")
	TObjectPtr<UTPSCharacterRecord> Record;


	//////////////////////////////////////////////////////
	// Volatile State

	// Character State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Character", Replicated)
	TEnumAsByte<ETPSCharacterBehaviorState> CurrentCharacterState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Character")
	TEnumAsByte<ETPSCharacterBehaviorState> PreviousCharacterState;
	//
	// Locomotion State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Locomotion", Replicated)
	TEnumAsByte<ETPSCharacterLocomotionState> CurrentLocomotionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Locomotion")
	TEnumAsByte<ETPSCharacterLocomotionState> PreviousLocomotionState;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Targeting", Replicated)
	FRotator TargetLookRotation;
	//
	// Should Target Location drive Target Rotation?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Targeting")
	bool IsTargetingLocation;
	//
	// Target Location - Provided by LOCAL Controller (Not Replicated)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Targeting")
	FVector TargetLookLocation;
	// Current Look Location - Iterps to TargetLocation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Targeting")
	FVector CurrentLookLocation;
	// Interp rate for CurrentLookLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Targeting")
	float LookTargetInterpRate = 0.8f;


	////////////////////////////////////////////////////////
	// Equipment State

	// Current Equipped Weapon (Synthetic)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSWeapon* GetEquippedWeapon() const;

	////////////////////////////////////////////////////////
	// Controller Input

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsBoosting;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsCrouchInputReceived;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsAiming;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsFiring;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsEquipping;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsReloading;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsInteracting;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	bool IsInMenu;

	//////////////////////////////////////////////////////
	// UI Visibility

	// Shows full diagnostic data to peer client/server
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool IsDebugEnabled = false;
	//
	// Shows name/health data to peer client/server
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool ShouldRenderUnitFrame = true;
	//
	// Shows simple debug data to peer client
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Render")
	bool ShouldRenderDebugFrame = true;




//~ ======================================================================== ~//
//  Blueprint Extensions
//~ ======================================================================== ~//
public:
	////////////////////////////////////////////////////////
	// Ability Extensions
	
	// Boost
	UFUNCTION(BlueprintCallable)
	void StartBoost();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBoostAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndBoost();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBoostAbilityEnd();
	//
	// Aim
	UFUNCTION(BlueprintCallable)
	void StartAim();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndAim();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimAbilityEnd();
	//
	// Fire Weapon
	UFUNCTION(BlueprintCallable)
	void StartFireWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFireWeaponAbilityStart();
	// Called when fire is actually performed to trigger animations
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFirePerformed();
	//
	UFUNCTION(BlueprintCallable)
	void EndFireWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFireWeaponAbilityEnd();
	//
	// EquipWeapon ("Ready")
	UFUNCTION(BlueprintCallable)
	void StartEquipWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipWeaponAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndEquipWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipWeaponAbilityEnd();
	//
	// UnEquipWeapon ("UnReady")
	UFUNCTION(BlueprintCallable)
	void StartUnEquipWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnEquipWeaponAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndUnEquipWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnEquipWeaponAbilityEnd();
	//
	// Reload Weapon
	UFUNCTION(BlueprintCallable)
	void StartReloadWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReloadWeaponAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndReloadWeapon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReloadWeaponAbilityEnd();
	//
	// Interact
	UFUNCTION(BlueprintCallable)
	void StartInteract();
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractAbilityStart();
	//
	UFUNCTION(BlueprintCallable)
	void EndInteract();
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractAbilityEnd();


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
	void ApplyCharacterState(const ETPSCharacterBehaviorState CharacterState);
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
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpeedModifierForLocomotionState(const ETPSCharacterLocomotionState LocomotionState);
	//
	// Apply calculated MovementSpeed
	UFUNCTION(BlueprintCallable)
	float UpdateCharacterSpeedForCurrentState();

	UFUNCTION(BlueprintCallable)
	bool IsActionActive() const;


	// Character Death
	UFUNCTION(BlueprintCallable)
	void PerformDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();




//~ ======================================================================== ~//
//  CONTROLLER POSSESSION
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
//  INVENTORY SYSTEM
//~ ======================================================================== ~//
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TObjectPtr<UTPSEquipmentManager> EquipmentManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TObjectPtr<UTPSCharacterInventory> Inventory;


//~ ======================================================================== ~//
//  MISC BEHAVIOR OVERRIDES
//~ ======================================================================== ~//

public:
	// Use Actor's Eyes at Mesh Location for detection and docking sweeps.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|Configuration")
	FName EyeSocketName;
	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	// Activate Ragdoll and die when outside world boundaries. (impl'd in BP)
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;


	// Apply damage via Ability System (if present)
	//virtual float TakeDamage(float damage, struct FDamageEvent const& event, AController* instigator, AActor* causer) override;
};