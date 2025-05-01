// (C) ToasterCat Studios 2025

#include "Character/TPSCharacter.h"

#include <Kismet/KismetSystemLibrary.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/TPSAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

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
	Configuration = CreateDefaultSubobject<UTPSCharacterConfiguration>(TEXT("DefaultConfiguration"));
	//
	Inventory = CreateDefaultSubobject<UTPSCharacterInventory>(TEXT("Inventory"));
	//
	EquipmentManager = CreateDefaultSubobject<UTPSEquipmentManager>(TEXT("EquipmentManager"));
	EquipmentManager->BindToMesh(GetMesh());



	//- Ability System ------------------------------------=
	//
	AbilitySystemComponent = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
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
	CurrentCharacterState = Casual;
	PreviousCharacterState = Incapacitated;
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
}

ATPSCharacter::~ATPSCharacter()
{
	// cleanup?
}

void ATPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ATPSCharacter, CurrentHealth);
	//DOREPLIFETIME(ATPSCharacter, CurrentArmor);

	DOREPLIFETIME(ATPSCharacter, CurrentLocomotionState);
	DOREPLIFETIME(ATPSCharacter, CurrentCharacterState);

	//DOREPLIFETIME(ATPSCharacter, MovementSpeedModifier);

	DOREPLIFETIME(ATPSCharacter, IsBoosting);
	DOREPLIFETIME(ATPSCharacter, IsCrouchInputReceived);
	DOREPLIFETIME(ATPSCharacter, IsAiming);
	DOREPLIFETIME(ATPSCharacter, IsFiring);
	DOREPLIFETIME(ATPSCharacter, IsEquipping);
	DOREPLIFETIME(ATPSCharacter, IsReloading);
	DOREPLIFETIME(ATPSCharacter, IsInteracting);
	DOREPLIFETIME(ATPSCharacter, IsInMenu);

	DOREPLIFETIME(ATPSCharacter, TargetLookRotation);
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipmentManager->BindToOwnerAbilitySystem(GetAbilitySystemComponent());
	if (HasAuthority())
	{
		EquipmentManager->Initialize();
		//EquipmentManager->EquipPrimary();

		SetupInitialAbilitiesAndEffects();
		if (ATPSPlayerState* playerState = GetPlayerState<ATPSPlayerState>())
		{
			GrantPlayerBasedAbilities();
		}
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	SyncAttributesFromGAS();
}


//~ ============================================================= ~//
//  GAME LOOP
//~ ============================================================= ~//


void ATPSCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//- Sync State from Input ----------------------------=
	//
	SyncAttributesFromGAS();
	//
	// Sync Character direction from Controller
	if (HasAuthority()) {
		TargetLookRotation = GetViewRotation();
	}

	//- Extend Input to Weapons -------------------------=
	//
	// Extend targeting data to current weapon
	ATPSWeapon* weapon = GetEquippedWeapon();
	if (IsValid(weapon))
	{
		weapon->TargetDirection = TargetLookRotation;
		weapon->TargetAccuracyTolerance = GetCurrentAccuracyTolerance();

		if (IsFiring)
		{
			if (weapon->CanFire())
			{
				weapon->Fire();
				OnFirePerformed();
			}
		}
	}

	//- Derive State from Input -------------------------=
	//
	ETPSLocomotionState evaluatedState = EvaluateLocomotionStateForCurrentInput();
	if (evaluatedState != CurrentLocomotionState)
	{
		ApplyLocomotionState(evaluatedState);
	}
	SyncComponentsFromState();

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
		NotifyDisplayWidgets.Broadcast();
		ShouldNotify = false;
	}
}

void ATPSCharacter::SyncComponentsFromState()
{
	UpdateCharacterSpeedForCurrentState();
}


//~ ============================================================= ~//
//  CONTROLLER POSSESSION
//   - Link w/ PlayerState's ASC
//~ ============================================================= ~//

void ATPSCharacter::PossessedBy(AController* NewController) { // server
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Character[%s]::PossessedBy()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Character[%s]::PossessedBy()"), *GetName());
	}

	if (HasAuthority())
	{
		GrantPlayerBasedAbilities();
	}
}
void ATPSCharacter::OnRep_PlayerState() { // client
	Super::OnRep_PlayerState();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Character[%s]::OnRep_PlayerState()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Character[%s]::OnRep_PlayerState()"), *GetName());
	}

	// A PlayerState was added (we were possesed)
	/*if (ATPSPlayerState* playerState = GetPlayerState<ATPSPlayerState>())
	{
		GrantPlayerBasedAbilities();
		//BindAbilitiesToInputComponent(playerState->PlayerAbilityInputBindings);
	}
	// A PlayerState was removed (we were UnPossessed)
	else
	{
		RevokePlayerBasedAbilities();
		//ReleaseAbilityBindingsFromInputComponent(GrantedPlayerBasedInputBindings);
	}*/
}

void ATPSCharacter::UnPossessed()
{
	if (HasAuthority())
	{
		RevokePlayerBasedAbilities();
	}
}



//~ ============================================================= ~//
//  SYNTHETIC GETTERS
//~ ============================================================= ~//

bool ATPSCharacter::IsAlive() const
{
	return (CurrentCharacterState != Incapacitated);
}
bool ATPSCharacter::IsCrouching() const
{
	return (CurrentLocomotionState == Crouching || CurrentLocomotionState == Prone);
}
bool ATPSCharacter::IsIdle() const
{
	return IdleSeconds >= Configuration->IdleThresholdSeconds;
}

FVector2D ATPSCharacter::GetCurrentAccuracyTolerance() const
{
	float accuracyModifier = 1.f;
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (IsValid(asc)) {
		accuracyModifier = asc->GetSet<UWeaponAttributeSet>()->GetAccuracyModifier();
	}

	return FVector2D(Configuration->BaseAccuracyTolerance, 
					 Configuration->BaseAccuracyTolerance) / accuracyModifier;
}

ATPSWeapon* ATPSCharacter::GetEquippedWeapon() const
{
	if (EquipmentManager->ActiveEquipmentSlot == None) { return nullptr; }

	return Cast<ATPSWeapon>(EquipmentManager->GetWeapon(EquipmentManager->ActiveEquipmentSlot));
}

//~ ============================================================= ~//
//  BEHAVIOR OPERATIONS
//~ ============================================================= ~//

void ATPSCharacter::ApplyLocomotionState(const ETPSLocomotionState LocomotionState)
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

void ATPSCharacter::ApplyCharacterState(const ETPSCharacterState CharacterState)
{
	if (CurrentCharacterState == CharacterState) { return; } // swallow redundant state changes

	PreviousCharacterState = CurrentCharacterState;
	CurrentCharacterState = CharacterState;

	// TODO: State Transitions Engine
	if (CurrentCharacterState == Combat)
	{
		//EquipmentManager->Ready();
	}
	else if (PreviousCharacterState == Combat && CurrentCharacterState != Incapacitated)
	{
		EquipmentManager->UnReady();
	}

	ShouldNotify = true;
}
void ATPSCharacter::RevertCharacterState() {
	ApplyCharacterState(PreviousCharacterState);
}

/**
 * https://www.notion.so/Game-Design-Document-GDD-670fda60cfeb41a089970b8fd240acaa?pvs=4#c31b4f8df636457b9783b951d60ecf03
 */
float ATPSCharacter::GetBaseSpeedForCharacterState(const ETPSCharacterState CharacterState)
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
float ATPSCharacter::GetSpeedModifierForLocomotionState(const ETPSLocomotionState LocomotionState)
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
	float baseSpeed = GetBaseSpeedForCharacterState(CurrentCharacterState);
	float locomotionStateModifier = GetSpeedModifierForLocomotionState(CurrentLocomotionState);

	// Set by GAS
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (IsValid(asc)) {
		MovementSpeedModifier = asc->GetNumericAttribute(UStandardAttributeSet::GetMovementSpeedModifierAttribute());
	}

	CurrentMaxWalkSpeed = baseSpeed * MovementSpeedModifier;

	// TODO: Lerp accel/decell?

	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	characterMovement->MaxWalkSpeed = CurrentMaxWalkSpeed;
	characterMovement->MaxWalkSpeedCrouched = CurrentMaxWalkSpeed;

	return CurrentMaxWalkSpeed;
}

ETPSLocomotionState ATPSCharacter::EvaluateLocomotionStateForCurrentInput()
{
	// TODO: Make this follow a strategy pattern based on current CharacterState
	//   Transitions based on allowed LocomotionStates for CharacterState

	// Character State Overrides
	if (CurrentCharacterState == Incapacitated)
	{
		return Ragdoll;
	}

	// Simplified State Tree (Casual & Combat)
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

bool ATPSCharacter::IsActionActive() const {
	return IsAiming
		|| IsFiring
		|| IsEquipping
		|| IsReloading
		|| IsInteracting
		|| IsInMenu;
}

void ATPSCharacter::PerformDeath()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] XXXXXXXXXXXX CHARACTER DEATH [%s]-[%s] XXXXXXXXXXXX"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] XXXXXXXXXXXX CHARACTER DEATH [%s]-[%s] XXXXXXXXXXXX"), *GetName());
	}


	ATPSPlayerController* controller = Cast<ATPSPlayerController>(GetController());
	if (IsValid(controller))
	{
		controller->NotifyPawnDeath();
	}

	OnDeath();
}


//~ ============================================================= ~//
//  Ability Extensions
//  - Update states dependent on ability activation. (IsBoosting, IsAiming, etc)
//  - Extend to BP listeners
//~ ============================================================= ~//

// - BOOST -//

void ATPSCharacter::StartBoost() {
	IsBoosting = true;
	OnBoostAbilityStart();
}
void ATPSCharacter::EndBoost() {
	IsBoosting = false;
	OnBoostAbilityEnd();
}

// - AIM -//

void ATPSCharacter::StartAim() {
	IsAiming = true;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->IsAiming = true;
	}

	OnAimAbilityStart();
}
void ATPSCharacter::EndAim() {
	IsAiming = false;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->IsAiming = false;
	}

	OnAimAbilityEnd();
}

// - FIRE WEAPON / USE EQUIPMENT -//

void ATPSCharacter::StartFireWeapon() {
	IsFiring = true;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->StartUse();
	}

	OnFireWeaponAbilityStart();
}
void ATPSCharacter::EndFireWeapon() {
	IsFiring = false;

	ATPSWeapon* weapon = GetEquippedWeapon();
	if (weapon != nullptr)
	{
		weapon->StopUse();
	}

	OnFireWeaponAbilityEnd();
}

// - EQUIP WEAPON -//

void ATPSCharacter::StartEquipWeapon() {
	IsEquipping = true;
	OnEquipWeaponAbilityStart();
}
void ATPSCharacter::EndEquipWeapon() {
	IsEquipping = false;
	OnEquipWeaponAbilityEnd();
}

// - UN-EQUIP WEAPON -//

void ATPSCharacter::StartUnEquipWeapon() {
	IsEquipping = true;
	OnUnEquipWeaponAbilityStart();
}
void ATPSCharacter::EndUnEquipWeapon() {
	IsEquipping = false;
	OnUnEquipWeaponAbilityEnd();
}

// - RELOAD WEAPON -//

void ATPSCharacter::StartReloadWeapon() {
	IsReloading = true;
	OnReloadWeaponAbilityStart();
}
void ATPSCharacter::EndReloadWeapon() {
	IsReloading = false;
	OnReloadWeaponAbilityEnd();
}

// - INTERACT -//

void ATPSCharacter::StartInteract() {
	IsInteracting = true;
	OnInteractAbilityStart();
}
void ATPSCharacter::EndInteract() {
	IsInteracting = false;
	OnInteractAbilityEnd();
}


// Blueprint Hook for on-fallout death animation.
void ATPSCharacter::FellOutOfWorld(const class UDamageType& dmgType) {
	OnFellOutOfWorld();
}


//~ ============================================================= ~//
//  ABILITY SYSTEM
//~ ============================================================= ~//


// Return local Character's ASC.
UAbilitySystemComponent* ATPSCharacter::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

// Should only be called from SERVER when initializing.
void ATPSCharacter::SetupInitialAbilitiesAndEffects()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing ASC for Character[%s]..."), *Name);

	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (! IsValid(asc)) {
		return;
	}
	asc->GetGameplayAttributeValueChangeDelegate(UCharacterHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	asc->GetGameplayAttributeValueChangeDelegate(UCharacterHealthAttributeSet::GetArmorAttribute())
		.AddUObject(this, &ThisClass::OnArmorAttributeChanged);
	asc->GetGameplayAttributeValueChangeDelegate(UStandardAttributeSet::GetMovementSpeedModifierAttribute())
		.AddUObject(this, &ThisClass::OnMovementAttributeChanged);

	//- Grant default abilities ---------------------------=
	//
	if (IsValid(InitialAbilitySet)) {
		BaseAbilitySpecHandles.Append(
			InitialAbilitySet->GrantAbilitiesToAbilitySystem(asc));
	}

	//- Initialize attributes -----------------------------=
	//
	if (IsValid(InitialGameplayEffect)) {
		asc->ApplyGameplayEffectToSelf(
			InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
			1,
			asc->MakeEffectContext());
	}

	UE_LOG(LogTemp, Log, TEXT("ASC for Character[%s] initialized."), *Name);
	for (auto ability : AbilitySystemComponent->GetActivatableAbilities())
	{
		UE_LOG(LogTemp, Log, TEXT("|--- [%s]::[%i]"), *ability.Ability->GetName(), ability.InputID);
	}
}

// Performed on Server
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
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (!IsValid(asc)) {
		return;
	}

	CurrentHealth = asc->GetNumericAttribute(UCharacterHealthAttributeSet::GetHealthAttribute());
	MaxHealth = asc->GetNumericAttribute(UCharacterHealthAttributeSet::GetHealthMaxAttribute());

	CurrentArmor = asc->GetNumericAttribute(UCharacterHealthAttributeSet::GetArmorAttribute());
	MaxArmor = asc->GetNumericAttribute(UCharacterHealthAttributeSet::GetArmorMaxAttribute());

	MovementSpeedModifier = asc->GetNumericAttribute(UStandardAttributeSet::GetMovementSpeedModifierAttribute());
}


// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	BindAbilitiesToInputComponent(BaseAbilityInputBindings);
	if (ATPSPlayerState* playerState = GetPlayerState<ATPSPlayerState>())
	{
		BindAbilitiesToInputComponent(playerState->PlayerAbilityInputBindings);
		GrantedPlayerBasedInputBindings = playerState->PlayerAbilityInputBindings;
	}
}

void ATPSCharacter::BindAbilitiesToInputComponent(FAbilityInputBindings bindings)
{
	if (UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(InputComponent)) {
		for (const FAbilityInputToInputActionBinding& binding : bindings.Bindings)
		{
			enhancedInput->BindAction(binding.InputAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputBindingPressedHandler, binding.AbilityInput);
			enhancedInput->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ThisClass::AbilityInputBindingReleasedHandler, binding.AbilityInput);
		}
	}
}

void ATPSCharacter::ReleaseAbilityBindingsFromInputComponent(FAbilityInputBindings bindings)
{
	// TODO: Might not be necessary? Does InputComponent deconstruct on UnPossess?
}

// EnhancedInput -> GAS plumbing
void ATPSCharacter::AbilityInputBindingPressedHandler(EAbilityInput abilityInput) {
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] CharacterASC::OnInputPressed[%i]"), abilityInput);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] CharacterASC::OnInputPressed[%i]"), abilityInput);
	}

	// Perform ability on local character
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
}
void ATPSCharacter::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput) {
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] CharacterASC::OnInputReleased[%i]"), abilityInput);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] CharacterASC::OnInputReleased[%i]"), abilityInput);
	}

	// Perform ability on local character
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(abilityInput));
}


void ATPSCharacter::GrantPlayerBasedAbilities()
{
	UE_LOG(LogTemp, Log, TEXT("Granting player-based abilities to Character[%s] ASC..."), *GetName());

	if (ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>())
	{
		GrantedPlayerBasedAbilitySpecHandles = AbilitySystemComponent->GrantAbilitiesFromAbilitySet(ps->PlayerAbilitySet);
		BindAbilitiesToInputComponent(ps->PlayerAbilityInputBindings);
	}

	UE_LOG(LogTemp, Log, TEXT("ASC for Character[%s] updated."), *Name);
	for (auto ability : AbilitySystemComponent->GetActivatableAbilities())
	{
		UE_LOG(LogTemp, Log, TEXT("|--- [%s]::[%i]"), *ability.Ability->GetName(), ability.InputID);
	}
}

void ATPSCharacter::RevokePlayerBasedAbilities()
{
	UE_LOG(LogTemp, Log, TEXT("Revoking player-based abilities from Character[%s] ASC..."), *GetName());

	AbilitySystemComponent->RevokeAbilitiesFromAbilitySystem(GrantedPlayerBasedAbilitySpecHandles);
	GrantedPlayerBasedAbilitySpecHandles.Empty();

	ReleaseAbilityBindingsFromInputComponent(GrantedPlayerBasedInputBindings);
	GrantedPlayerBasedInputBindings.Bindings.Empty();

	/*UE_LOG(LogTemp, Log, TEXT("ASC for Character[%s] updated."), *Name);
	for (auto ability : AbilitySystemComponent->GetActivatableAbilities())
	{
		UE_LOG(LogTemp, Log, TEXT("|--- [%s]::[%i]"), *ability.Ability->GetName(), ability.InputID);
	}*/
}




//~ ============================================================= ~//
//  MISC CONFIGURATION
//~ ============================================================= ~//


// AI Enhancement - Detection FOV is rooted to Character's HEAD.
void ATPSCharacter::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation(EyeSocketName);
	Rotation = GetMesh()->GetSocketRotation(EyeSocketName);
}

// Apply generic damage to ASC
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