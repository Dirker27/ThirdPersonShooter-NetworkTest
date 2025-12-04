// (C) ToasterCat Studios 2025

#include "Character/TPSCharacter.h"

#include "Game/TPSGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/TPSAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Character/TPSCharacterInstance.h"
#include "Player/TPSPlayerController.h"
#include "Player/TPSPlayerState.h"



//~ ======================================================================= ~//
//  INITIALIZATION AND SETUP
//~ ======================================================================= ~//

ATPSCharacter::ATPSCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	//- Sub-Components ------------------------------------=
	//
	Inventory = CreateDefaultSubobject<UTPSCharacterInventory>(TEXT("Inventory"));
	//
	EquipmentManager = CreateDefaultSubobject<UTPSEquipmentManager>(TEXT("EquipmentManager"));
	EquipmentManager->BindToMesh(GetMesh());  // <- Bind early to allow for visual adjustments to MountPoints



	//- Ability System ------------------------------------=
	//
	AbilitySystem = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	//
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributes"));
	CharacterHealthAttributes = CreateDefaultSubobject<UCharacterHealthAttributeSet>(TEXT("HealthAttributes"));
	WeaponAttributes = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("WeaponAttributes"));


	//- Default Values ------------------------------------=
	//
	// Used for automated behavior testing
	//   Will be overridden in Blueprints or in BeginPlay()
	//
	CurrentHealth = 100;
	MaxHealth = 100;
	CurrentArmor = 100;
	MaxArmor = 100;
	//
	MovementSpeedModifier = 1.0;
	CurrentMaxWalkSpeed = 200;
	//
	CurrentBehaviorState = Casual;
	PreviousBehaviorState = Incapacitated;
	CurrentLocomotionState = Standing;
	PreviousLocomotionState = Crouching;
	//
	IsBoosting = false;
	IsCrouchInputReceived = false;
	IsAiming = false;
	IsFiring = false;
	IsInteracting = false;
	IsInMenu = false;
	TargetLookRotation = FRotator::ZeroRotator;

	CanDie = true;
	HasDeathTriggered = false;
}

ATPSCharacter::~ATPSCharacter()
{
	// cleanup?
}

void ATPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSCharacter, CharacterInstance);

	DOREPLIFETIME(ATPSCharacter, CurrentLocomotionState);
	DOREPLIFETIME(ATPSCharacter, CurrentBehaviorState);
	DOREPLIFETIME(ATPSCharacter, Stance);

	DOREPLIFETIME(ATPSCharacter, IsBoosting);
	DOREPLIFETIME(ATPSCharacter, IsCrouchInputReceived);
	DOREPLIFETIME(ATPSCharacter, IsAiming);
	DOREPLIFETIME(ATPSCharacter, IsFiring);
	DOREPLIFETIME(ATPSCharacter, IsEquipping);
	DOREPLIFETIME(ATPSCharacter, IsReloading);
	DOREPLIFETIME(ATPSCharacter, IsInteracting);
	DOREPLIFETIME(ATPSCharacter, IsTransitioning);
	DOREPLIFETIME(ATPSCharacter, IsInMenu);

	//DOREPLIFETIME(ATPSCharacter, TargetLookLocation);
	DOREPLIFETIME(ATPSCharacter, TargetLookRotation);
	DOREPLIFETIME(ATPSCharacter, HasDeathTriggered);
}


void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Log, TEXT("Character[%s]::BeginPlay"), *GetName());

	// Init ASC
	AbilitySystem->InitAbilityActorInfo(this, this); // <- required for all ASC consumers
	if (HasAuthority())
	{
		SetupAbilitySystem();

		// If pawn is spawned w/ a PlayerState, add these abilities immediately
		//   ( PossessedBy() and OnRep_PlayerState() may not fire )
		if (ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>())
		{
			AbilitySystem->GrantPlayerBasedAbilities(ps->PlayerAbilitySet);
		}
	}
	
	// Init EquipmentManager (consumes and distributes ASC)
	EquipmentManager->BindToOwnerAbilitySystem(AbilitySystem);
	if (HasAuthority())
	{
		EquipmentManager->Initialize();
	}


	SyncAttributesFromGAS();
	CurrentLookLocation = TargetLookLocation;


	/*if (IsValid(CharacterInstance))
	{
		Initialize();
	}*/
}

void ATPSCharacter::Initialize()
{
	if (!IsValid(CharacterInstance))
	{
		UE_LOG(LogTemp, Log, TEXT("FAILED to initialize Character[%s] - invalid Instance provided."), *GetName());
		return;
	}

	// TODO: Sync Attributes

	ShouldNotify = true;

	// Extend to Blueprints
	//   Initialize components with Instance-defined behavior
	OnInitialize();
}


void ATPSCharacter::BindToCharacterInstance(UTPSCharacterInstance* instance)
{
	if (!IsValid(instance)) { return; }

	CharacterInstance = instance;
	Initialize();
}

// Called from CLIENT on update
void ATPSCharacter::OnRep_CharacterInstance()
{
	Initialize();
}



//~ ======================================================================== ~//
//  GAME LOOP
//  ---------
//  - Sync locally-accessible attributes from GAS
//  - Evaluate if current state should change for current controller input
//  - Extend controller input to sub-components
//  - If changes affect UI display, notify UI widgets - chains to UI::OnUpdate()
//~ ======================================================================== ~//





void ATPSCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);


	//- Sync state from AbilitySystem ----------------------------=
	//
	SyncAttributesFromGAS();


	// DEATH CHECK - Server-Only
	if (HasAuthority())
	{
		if (IsDeathConditionMet() && !HasDeathTriggered)
		{
			ATPSGameMode* gameMode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(this));
			if (IsValid(CharacterInstance)) { gameMode->KillCharacter(CharacterInstance->CharacterID); }
			Die();
		}
	}


	//- Sync Character direction from Controller --------------------------=
	//
	if (IsLocallyControlled())
	{
		IsTargetingLocation = true;
	}
	else
	{
		IsTargetingLocation = false;
	}
	CurrentLookLocation = FMath::Lerp(CurrentLookLocation, TargetLookLocation, LookTargetInterpRate);
	//CurrentLookLocation = FMath::FInterpTo(CurrentLookLocation, TargetLookLocation, deltaTime, LookTargetInterpRate);
	if (IsTargetingLocation) {
		FVector delta = CurrentLookLocation - GetActorLocation();
		TargetLookRotation = delta.Rotation();
	}
	else {
		FRotator vr = GetViewRotation();
		if (vr != GetActorRotation()) // Guards against "noise" where remote client only sees controller value when input is active
		{
			TargetLookRotation = vr;
		}
	}


	//- Extend Input to Weapons -------------------------=
	//
	// Extend targeting data to current weapon
	ATPSWeapon* weapon = GetEquippedWeapon();
	if (IsValid(weapon) && CurrentBehaviorState == Combat)
	{
		weapon->TargetLocation = CurrentLookLocation;
		weapon->TargetAccuracyTolerance = GetCurrentAccuracyTolerance();

		if (IsFiring)
		{
			if (weapon->CanFire())
			{
				if (HasAuthority()) {
					weapon->Fire();
					OnFirePerformed();
				}
			}
		}
	}


	//- Derive State from Input -------------------------=
	//
	ETPSCharacterLocomotionState evaluatedState = EvaluateLocomotionStateForCurrentInput();
	if (evaluatedState != CurrentLocomotionState)
	{
		ApplyLocomotionState(evaluatedState);
	}
	SyncComponentsFromState();
	//
	if (IsActionActive() || GetVelocity().Size() > 0.1)
	{
		IdleSeconds = 0;
	}
	else
	{
		IdleSeconds += deltaTime;
	}


	//- Broadcast to UI Listeners -----------------------=
	//
	if (ShouldNotify) {
		CharacterActorUpdate.Broadcast();
		ShouldNotify = false;
	}
}

void ATPSCharacter::SyncComponentsFromState()
{
	UpdateCharacterSpeedForCurrentState();
}




//~ ============================================================= ~//
//  SYNTHETIC GETTERS
//~ ============================================================= ~//

FTPSCharacterID ATPSCharacter::GetCharacterID() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->CharacterID
		: FTPSCharacterID();
}

FTPSOperatorIdentity ATPSCharacter::GetIdentity() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->Identity
		: FTPSOperatorIdentity();
}

FTPSCharacterConfigurationData ATPSCharacter::GetConfiguration() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->Configuration
		: FTPSCharacterConfigurationData();
}

ETPSTeamID ATPSCharacter::GetAssignedTeamID() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->GetAssignedTeamID()
		: ETPSTeamID::UNAFFILIATED;
}

UTPSTeamInstance* ATPSCharacter::GetAssignedTeam() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->GetAssignedTeam()
		: nullptr;
}

UTPSCommandUnit* ATPSCharacter::GetAssignedUnit() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->GetAssignedUnit()
		: nullptr;
}

UTPSArmyInstance* ATPSCharacter::GetAssignedArmy() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->GetAssignedArmy()
		: nullptr;
}


bool ATPSCharacter::IsInitialized() const
{
	return IsValid(CharacterInstance);
}

bool ATPSCharacter::IsAlive() const
{
	return IsValid(CharacterInstance)
		? CharacterInstance->IsAlive
		: (CurrentBehaviorState != Incapacitated);
}




//~ ============================================================= ~//
//  STATE-DEPENDENT SYNTHETIC GETTERS
//~ ============================================================= ~//

bool ATPSCharacter::IsCrouching() const
{
	return (CurrentLocomotionState == Crouching || CurrentLocomotionState == Prone);
}
bool ATPSCharacter::IsIdle() const
{
	return IdleSeconds >= GetConfiguration().IdleThresholdSeconds;
}

FVector2D ATPSCharacter::GetCurrentAccuracyTolerance() const
{
	float accuracyModifier = 1.f;
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (IsValid(asc)) {
		accuracyModifier = asc->GetSet<UWeaponAttributeSet>()->GetAccuracyModifier();
	}

	return FVector2D(GetConfiguration().BaseAccuracyTolerance,
		GetConfiguration().BaseAccuracyTolerance) / accuracyModifier;
}

ATPSWeapon* ATPSCharacter::GetEquippedWeapon() const
{
	if (EquipmentManager->ActiveEquipmentSlot == None) { return nullptr; }

	return Cast<ATPSWeapon>(EquipmentManager->GetWeapon(EquipmentManager->ActiveEquipmentSlot));
}




//~ ======================================================================== ~//
//  BEHAVIOR OPERATIONS
//  -------------------
//  - Determine if current states should be modified based on input
//  - Define behavior on death
//~ ======================================================================== ~//


bool ATPSCharacter::IsDeathConditionMet()
{
	if (CanDie && CurrentHealth <= 0)
	{
		return true;
	}
	return false;
}



void ATPSCharacter::SetTargetLocation(FVector targetLocation)
{
	TargetLookLocation = targetLocation;
}



void ATPSCharacter::ApplyLocomotionState(const ETPSCharacterLocomotionState LocomotionState)
{
	if (CurrentLocomotionState == LocomotionState) { return; } // swallow redundant state changes

	PreviousLocomotionState = CurrentLocomotionState;
	CurrentLocomotionState = LocomotionState;

	// Cascade to inner components (adjusts collider height)
	if (CurrentLocomotionState == Crouching) {
		Crouch();
	}
	else if (PreviousLocomotionState == Crouching)
	{
		UnCrouch();
	}

	ShouldNotify = true;
}
void ATPSCharacter::RevertLocomotionState() {
	ApplyLocomotionState(PreviousLocomotionState);
}

void ATPSCharacter::ApplyBehaviorState(const ETPSCharacterBehaviorState CharacterState)
{
	if (CurrentBehaviorState == CharacterState) { return; } // swallow redundant state changes

	PreviousBehaviorState = CurrentBehaviorState;
	CurrentBehaviorState = CharacterState;

	// TODO: TPSGameState Transitions Engine
	if (CurrentBehaviorState == Combat)
	{
		//EquipmentManager->Ready();
	}
	else if (PreviousBehaviorState == Combat && CurrentBehaviorState != Incapacitated)
	{
		EquipmentManager->UnReady();
	}

	ShouldNotify = true;
}
void ATPSCharacter::RevertCharacterState() {
	ApplyBehaviorState(PreviousBehaviorState);
}

void ATPSCharacter::InterruptIdle()
{
	IdleSeconds = 0;

	ShouldNotify = true;
}



/**
 * https://www.notion.so/Game-Design-Document-GDD-670fda60cfeb41a089970b8fd240acaa?pvs=4#c31b4f8df636457b9783b951d60ecf03
 */
float ATPSCharacter::GetBaseSpeedForCharacterState(const ETPSCharacterBehaviorState CharacterState)
{
	switch (CharacterState) {
	case Casual:
		return 200;
	case Combat:
		return 300;
	case Injured:
		return 100;
	case Incapacitated:
	default:
		return 0;
	}
}

/**
 * We actually set this with GameplayEffects. Leaving this here for legacy context
 *   and documentation link:
 * 
 * https://www.notion.so/Game-Design-Document-GDD-670fda60cfeb41a089970b8fd240acaa?pvs=4#c31b4f8df636457b9783b951d60ecf03 
 */
float ATPSCharacter::GetSpeedModifierForLocomotionState(const ETPSCharacterLocomotionState LocomotionState)
{
	switch (LocomotionState) {
	case Standing:
		return 1.0;
	case Crouching:
		return 0.5;
	case Prone:
		return 0.1;
	case Sprinting:
		return 2.0;
	case Ragdoll:
	default:
		return 0;
	}
}

float ATPSCharacter::UpdateCharacterSpeedForCurrentState()
{
	float baseSpeed = GetBaseSpeedForCharacterState(CurrentBehaviorState);
	float locomotionStateModifier = GetSpeedModifierForLocomotionState(CurrentLocomotionState);

	// Set by GAS
	if (IsValid(AbilitySystem)) {
		MovementSpeedModifier = AbilitySystem->GetNumericAttribute(UStandardAttributeSet::GetMovementSpeedModifierAttribute());
	}

	CurrentMaxWalkSpeed = baseSpeed * MovementSpeedModifier;

	// TODO: Lerp accel/decell?

	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	characterMovement->MaxWalkSpeed = CurrentMaxWalkSpeed;
	characterMovement->MaxWalkSpeedCrouched = CurrentMaxWalkSpeed;

	return CurrentMaxWalkSpeed;
}

ETPSCharacterLocomotionState ATPSCharacter::EvaluateLocomotionStateForCurrentInput()
{
	// TODO: Make this follow a strategy pattern based on current CharacterState
	//   Transitions based on allowed LocomotionStates for CharacterState

	// Character TPSGameState Overrides
	if (CurrentBehaviorState == Incapacitated)
	{
		return Ragdoll;
	}

	// Simplified TPSGameState Tree (Casual & Combat)
	if (IsCrouchInputReceived)
	{
		return Crouching;
	}
	if (IsBoosting && !IsActionActive())
	{
		return Sprinting;
	}

	return Standing;
}

bool ATPSCharacter::IsActionActive() const
{
	return IsAiming
		|| IsFiring
		|| IsEquipping
		|| IsReloading
		|| IsInteracting
		|| IsTransitioning
		|| IsInMenu;
}


void ATPSCharacter::Die_Implementation()
{
	if (!HasDeathTriggered) {
		HasDeathTriggered = true;
		StartDeath();
	}
}


void ATPSCharacter::StartDeath_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] XXXXXXXXXXXX CHARACTER DEATH [%s] XXXXXXXXXXXX"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] XXXXXXXXXXXX CHARACTER DEATH [%s] XXXXXXXXXXXX"), *GetName());
	}

	ApplyBehaviorState(Incapacitated);

	OnDeathStart();

	ShouldNotify = true;
}


void ATPSCharacter::CompleteDeath_Implementation()
{
	//ApplyBehaviorState(Incapacitated);

	// Notify Player (iff controlled)
	if (ATPSPlayerController* controller = GetController<ATPSPlayerController>())
	{
		controller->NotifyPawnDeath();
	}

	OnDeathComplete();

	ShouldNotify = true;
}



//~ ======================================================================== ~//
//  Ability Extensions
//  ------------------
//  - Update states dependent on ability activation. (IsBoosting, IsAiming, etc)
//  - Extend to BP listeners
//~ ======================================================================== ~//




// - BOOST -//

void ATPSCharacter::StartBoost()
{
	IsBoosting = true;
	OnBoostAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndBoost()
{
	IsBoosting = false;
	OnBoostAbilityEnd();

	ShouldNotify = true;
}

// - AIM -//

void ATPSCharacter::StartAim()
{
	IsAiming = true;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->IsAiming = true;
	}

	OnAimAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndAim()
{
	IsAiming = false;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->IsAiming = false;
	}

	OnAimAbilityEnd();

	ShouldNotify = true;
}

// - FIRE WEAPON / USE EQUIPMENT -//

void ATPSCharacter::StartFireWeapon()
{
	IsFiring = true;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->StartUse();
	}

	OnFireWeaponAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndFireWeapon()
{
	IsFiring = false;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->StopUse();
	}

	OnFireWeaponAbilityEnd();

	ShouldNotify = true;
}

// - EQUIP WEAPON -//

void ATPSCharacter::StartEquipWeapon()
{
	IsEquipping = true;
	OnEquipWeaponAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndEquipWeapon() {
	IsEquipping = false;
	OnEquipWeaponAbilityEnd();

	ShouldNotify = true;
}

// - UN-EQUIP WEAPON -//

void ATPSCharacter::StartUnEquipWeapon()
{
	IsEquipping = true;
	OnUnEquipWeaponAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndUnEquipWeapon()
{
	IsEquipping = false;
	OnUnEquipWeaponAbilityEnd();

	ShouldNotify = true;
}

// - RELOAD WEAPON -//

void ATPSCharacter::StartReloadWeapon()
{
	IsReloading = true;
	OnReloadWeaponAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndReloadWeapon()
{
	IsReloading = false;
	OnReloadWeaponAbilityEnd();

	ShouldNotify = true;
}

// - INTERACT -//

void ATPSCharacter::StartInteract()
{
	IsInteracting = true;
	OnInteractAbilityStart();

	ShouldNotify = true;
}
void ATPSCharacter::EndInteract()
{
	IsInteracting = false;
	OnInteractAbilityEnd();

	ShouldNotify = true;
}

// - SWITCH STANCE -//

void ATPSCharacter::StartSwitchStance()
{
	Stance = (Stance == ETPSCharacterStance::RightHanded)
		? ETPSCharacterStance::LeftHanded
		: ETPSCharacterStance::RightHanded;

	IsTransitioning = true;
	OnSwitchStanceAbilityStart();
}
void ATPSCharacter::EndSwitchStance()
{
	IsTransitioning = false;
	OnSwitchStanceAbilityEnd();
}

// - ENTER / EXIT MENU -//

void ATPSCharacter::StartUseMenu()
{
	IsInMenu = true;
	OnUseMenuAbilityStart();
}
void ATPSCharacter::EndUseMenu()
{
	IsInMenu = false;
	OnUseMenuAbilityEnd();
}



// Blueprint Hook for on-fallout death animation.
void ATPSCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	OnFellOutOfWorld();
}





//~ ======================================================================== ~//
//  CONTROLLER POSSESSION
//  ---------------------
//  - Grant player-based abilities to AbilitySystem and bind to InputComponent
//~ ======================================================================== ~//




void ATPSCharacter::PossessedBy(AController* NewController) // server
{ 
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Character[%s]::PossessedBy(Controller[%s])"), *GetName(), *NewController->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Character[%s]::PossessedBy(Controller[%s])"), *GetName(), *NewController->GetName());
	}

	if (ATPSPlayerState* ps = NewController->GetPlayerState<ATPSPlayerState>())
	{
		if (HasAuthority())
		{
			AbilitySystem->GrantPlayerBasedAbilities(ps->PlayerAbilitySet);
		}
	}

	ShouldNotify = true;
}

void ATPSCharacter::OnRep_PlayerState() // client
{ 
	Super::OnRep_PlayerState();

	/*if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Character[%s]::OnRep_PlayerState()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Character[%s]::OnRep_PlayerState()"), *GetName());
	}*/
	ShouldNotify = true;
}

void ATPSCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (HasAuthority())
	{
		AbilitySystem->RevokePlayerBasedAbilities();
	}
	ShouldNotify = true;
}

void ATPSCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}





//~ ======================================================================== ~//
//  ABILITY SYSTEM
//  --------------
//  - Grant character-based abilities and apply initial effects
//  - Bind InputComponent to mapped GameplayAbilities
//  - Sync local attributes to GAS-driven attributes (health, movement speed, etc.)
//  - Grant / Revoke player-based abilities (can only have 1 set)
//~ ======================================================================== ~//






// Return local Character's AbilitySystem.
UAbilitySystemComponent* ATPSCharacter::GetAbilitySystemComponent() const {
	return AbilitySystem;
}

// Should only be called from SERVER when initializing.
void ATPSCharacter::SetupAbilitySystem()
{
	if (!IsValid(AbilitySystem)) { return; }
	//UE_LOG(LogTemp, Log, TEXT("Initializing ASC for Character[%s]..."), *Identity->Guid.ToString());

	AbilitySystem->InitializeBaseAbilitiesAndEffects();

	// Configure attributes (OnChange listeners)
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UCharacterHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UCharacterHealthAttributeSet::GetArmorAttribute())
		.AddUObject(this, &ThisClass::OnArmorAttributeChanged);
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UStandardAttributeSet::GetMovementSpeedModifierAttribute())
		.AddUObject(this, &ThisClass::OnMovementAttributeChanged);

	// Print
	//UE_LOG(LogTemp, Log, TEXT("ASC for Character[%s] initialized."), *Identity->Guid.ToString());
	/*for (auto ability : AbilitySystem->GetActivatableAbilities())
	{
		UE_LOG(LogTemp, Log, TEXT("|--- [%s]::[%i]"), *ability.Ability->GetName(), ability.InputID);
	}*/
}

// OnChange listeners performed on SERVER
void ATPSCharacter::OnArmorAttributeChanged(const FOnAttributeChangeData& data) {
	CurrentArmor = data.NewValue;
	ShouldNotify = true;
}
void ATPSCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& data) {
	CurrentHealth = data.NewValue;
	ShouldNotify = true;
}
void ATPSCharacter::OnMovementAttributeChanged(const FOnAttributeChangeData& data) {
	MovementSpeedModifier = data.NewValue;
	ShouldNotify = true;
}

// Sync clients from Server-driven GAS updates.
void ATPSCharacter::SyncAttributesFromGAS()
{
	if (!IsValid(AbilitySystem)) { return; }

	CurrentHealth = AbilitySystem->GetNumericAttribute(UCharacterHealthAttributeSet::GetHealthAttribute());
	MaxHealth = AbilitySystem->GetNumericAttribute(UCharacterHealthAttributeSet::GetHealthMaxAttribute());

	CurrentArmor = AbilitySystem->GetNumericAttribute(UCharacterHealthAttributeSet::GetArmorAttribute());
	MaxArmor = AbilitySystem->GetNumericAttribute(UCharacterHealthAttributeSet::GetArmorMaxAttribute());

	MovementSpeedModifier = AbilitySystem->GetNumericAttribute(UStandardAttributeSet::GetMovementSpeedModifierAttribute());
}


// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	//UE_LOG(LogTemp, Log, TEXT("Character[%s]::SetupInputComponent()"), *Identity->Guid.ToString());
	AbilitySystem->BindToInputComponent(playerInputComponent);
}




//~ ======================================================================== ~//
//  MISC CONFIGURATION
//~ ======================================================================== ~//




// AI Enhancement - Detection FOV is rooted to Character's HEAD.
void ATPSCharacter::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation(EyeSocketName);
	Rotation = GetMesh()->GetSocketRotation(EyeSocketName);
}

// Apply generic damage to AbilitySystem
/*float ATPSCharacter::TakeDamage(float damage, struct FDamageEvent const& event, AController* instigator, AActor* causer)
{
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (IsValid(asc))
	{
		//swallow?
	}
	else
	{
		float healthDamage = damage;
		if (CurrentArmor > 0)
		{
			healthDamage = damage - CurrentArmor;

			CurrentArmor -= damage;
			if (CurrentArmor < 0) { CurrentArmor = 0; }
		}
		if (healthDamage > 0)
		{
			CurrentHealth -= healthDamage;
			if (CurrentHealth < 0) { CurrentHealth = 0; }
		}
	}

	return damage;
}*/




//~ ============================================================= ~//
//  END
//~ ============================================================= ~//