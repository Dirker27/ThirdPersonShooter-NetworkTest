// (C) ToasterCat Studios 2025

#include "Character/TPSCharacter.h"

#include <Kismet/KismetSystemLibrary.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

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
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
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
	UpdateInputContextForCurrentState();
}


//~ ============================================================= ~//
//  CONTROLLER POSSESSION
//   - Link w/ PlayerState's ASC
//~ ============================================================= ~//

void ATPSCharacter::PossessedBy(AController* NewController) { // server
	Super::PossessedBy(NewController);

	// Initialize GAS
	//BindToPlayerAbilitySystem();
	//SetupInitialAbilitiesAndEffects();
	SyncAttributesFromGAS();
}
void ATPSCharacter::OnRep_PlayerState() { // client
	Super::OnRep_PlayerState();

	// Initialize GAS
	//BindToPlayerAbilitySystem();
	SyncAttributesFromGAS();
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

	return Cast<ATPSWeapon>(EquipmentManager->GetItemFromEquipmentSlot(EquipmentManager->ActiveEquipmentSlot));
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



void ATPSCharacter::UpdateInputContextForCurrentState()
{
	/*player = GetPlayer()
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), Priority);
			}
		}
	}*/
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
	ATPSPlayerController* controller = Cast<ATPSPlayerController>(GetController());
	if (IsValid(controller))
	{
		controller->NotifyPawnDeath();
	}

	OnDeath();
}


AActor* ATPSCharacter::LineTrace(const UObject* WorldContextObject) {
	AActor* hitActor = NULL;

	FVector startLoc = GetActorLocation();

	FVector forward = GetActorForwardVector();
	FVector endLoc = startLoc + (forward * 1000.f);

	ETraceTypeQuery channel = TraceTypeQuery_MAX;
	TArray<AActor*> actorsToIgnore;
	EDrawDebugTrace::Type debugTrace = EDrawDebugTrace::Type::ForOneFrame;
	FHitResult hitResult;

	bool isHit = UKismetSystemLibrary::LineTraceSingle(WorldContextObject, startLoc, endLoc,
		channel, false, actorsToIgnore, debugTrace,
		hitResult,
		true,
		FLinearColor::Red, FLinearColor::Green, 5.f);

	if (isHit) {
		hitActor = hitResult.GetActor();
	}

	return hitActor;
}


//~ ============================================================= ~//
//  Ability Extensions
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

// Return Player's ASC if possessed.
UAbilitySystemComponent* ATPSCharacter::GetPlayerAbilitySystemComponent() const {
	ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>();
	return ps ? ps->GetAbilitySystemComponent() : nullptr;
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
		InitiallyGrantedAbilitySpecHandles.Append(
			InitialAbilitySet->GrantAbilitiesToAbilitySystem(asc));
	}

	//- Initialize attributes -----------------------------=
	//
	if (IsValid(InitialGameplayEffect)) {
		asc->ApplyGameplayEffectToSelf(
			InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
			0,
			asc->MakeEffectContext());
	}

	UE_LOG(LogTemp, Log, TEXT("ASC for Character[%s] initialized."), *Name);
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


// Ability System Wiring (from TPSPlayerState)
void ATPSCharacter::BindToPlayerAbilitySystem()
{
	if (ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>()) {
		ps->GetAbilitySystemComponent()->InitAbilityActorInfo(ps, this);
	}

	if (HasAuthority() || IsLocallyControlled()) {
		// init
	}
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent* playerEnhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent)) {
		for (const FAbilityInputToInputActionBinding& binding : AbilityInputBindings.Bindings)
		{
			playerEnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputBindingPressedHandler, binding.AbilityInput);
			playerEnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ThisClass::AbilityInputBindingReleasedHandler, binding.AbilityInput);
		}
	}
}

// EnhancedInput -> GAS plumbing
void ATPSCharacter::AbilityInputBindingPressedHandler(EAbilityInput abilityInput) {
	UE_LOG(LogTemp, Log, TEXT("OnInputPressed[%i]"), abilityInput);

	// Perform ability on local character
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));

	// Extend input to Player's ASC
	if (ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>()) {
		ps->GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
	}
}
void ATPSCharacter::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput) {
	UE_LOG(LogTemp, Log, TEXT("OnInputReleased[%i]"), abilityInput);

	// Perform ability on local character
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(abilityInput));

	// Extend input to player's ASC
	if (ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>()) {
		ps->GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<uint32>(abilityInput));
	}
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