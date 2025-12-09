// (C) ToasterCat Studios 2025

#include "Player/TPSPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Game/TPSGameMode.h"
#include "Pawn/TPSPawn.h"
#include "Util/TPSFunctionLibrary.h"

ATPSPlayerController::ATPSPlayerController()
{
	//bHidden = false;
#if WITH_EDITORONLY_DATA
	//bHiddenEd = false;
#endif // WITH_EDITORONLY_DATA
	//SetHidden(false);
}

// PlayerControllers are NOT replicated to peer clients
//   This sync will only ever be between Server<->OwningClient
// ie:
//     Player1 <-> Server == Controller1
//     Player2 <-> Server == Controller2
//     Player1 <-> Player2 != Controller1 || Controller2
void ATPSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ControllerState);
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::BeginPlay()"));

	BindConsoleCallbacks();
}

// Executes ON OWNING CLIENT when PlayerState is connected to Controller from Server
void ATPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::OnRep_PlayerState()"));

	if (auto p = BoundPlayer())
	{
		BindControllerToPlayer(p);
	}
}

// Executes ON OWNING CLIENT when Pawn is connected to Controller from Server
void ATPSPlayerController::OnRep_Pawn()
{
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::OnRep_Pawn()"));
	Super::OnRep_Pawn();

	OnPawnPossessionChanged();
}

void ATPSPlayerController::OnPawnPossessionChanged()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] TPSPlayerController::OnPawnPossessionChanged()"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] TPSPlayerController::OnPawnPossessionChanged()"));
	}


	// Unbind from Old
	if (auto c = BoundCharacter.Get())
	{
		UnBindControllerFromCharacter(c);
	}
	if (auto p = BoundPawn.Get())
	{
		UnBindControllerFromPawn(p);
	}



	// Bind to New (if present)
	if (auto c = PossessedCharacter())
	{
		BindControllerToCharacter(c);
	}
	else if (auto p = PossessedPawn())
	{
		BindControllerToPawn(p);
	}
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::SetupInputComponent()"));
}

void ATPSPlayerController::SetControllerState(const ETPSControllerState state)
{
	ControllerState = state;
	ControllerStateUpdate.Broadcast();
}

void ATPSPlayerController::OnRep_ControllerState()
{
	ControllerStateUpdate.Broadcast();
}

void ATPSPlayerController::BindControllerToPlayer(ATPSPlayerState* newPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::BindControllerToPlayer([%s])"), *newPlayer->GetName());
	OnControllerBoundToPlayer(newPlayer);
}

void ATPSPlayerController::BindControllerToCharacter(ATPSCharacter* newCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::BindControllerToCharacter([%s])"), *newCharacter->GetName());

	if (auto instance = newCharacter->GetCharacterInstance())
	{
		instance->BindInstanceToPlayer(BoundPlayer());
	}

	OnControllerBoundToCharacter(newCharacter);
	BoundCharacter = newCharacter; // Set after to avoid un-binding callbacks on a bad ref if BP binding fails
}
void ATPSPlayerController::UnBindControllerFromCharacter(ATPSCharacter* oldCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::UnBindControllerFromCharacter([%s])"), *oldCharacter->GetName());

	if (auto instance = oldCharacter->GetCharacterInstance())
	{
		instance->UnBindInstanceFromPlayer(BoundPlayer());
	}

	BoundCharacter = nullptr;
	OnControllerUnBoundFromCharacter(oldCharacter);
}

void ATPSPlayerController::BindControllerToPawn(ATPSPawn* newPawn)
{
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::BindControllerToPawn([%s])"), *newPawn->GetName());

	OnControllerBoundToPawn(newPawn);
	BoundPawn = newPawn; // Set after to avoid un-binding callbacks on a bad ref if BP binding fails
}
void ATPSPlayerController::UnBindControllerFromPawn(ATPSPawn* oldPawn)
{
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::UnBindControllerFromPawn([%s])"), *oldPawn->GetName());

	BoundPawn = nullptr;
	OnControllerUnBoundFromPawn(oldPawn);
}

void ATPSPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (auto character  = PossessedCharacter())
	{
		character->SetTargetLocation(GetCameraTargetLocation());
	}
	else if (auto p = PossessedPawn())
	{
		p->SetTargetLocation(GetCameraTargetLocation());
	}
}


//~ ====================================================================== ~//
//- BEHAVIOR OPERATIONS
//~ ====================================================================== ~//

FVector ATPSPlayerController::GetCameraTargetLocation() const
{
	FVector playerLoc;
	FRotator cameraRot;
	GetPlayerViewPoint(playerLoc, cameraRot);

	FVector cameraTargetLoc = playerLoc + (cameraRot.Vector() * 10000);

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetPawn());

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(this, playerLoc, cameraTargetLoc,
		TraceTypeQuery_MAX, false, actorsToIgnore, EDrawDebugTrace::Type::None,
		hitResult,
		true,
		FLinearColor::Red, FLinearColor::Green, 5.f);

	return hitResult.IsValidBlockingHit()
		? hitResult.ImpactPoint
		: cameraTargetLoc;
}


void ATPSPlayerController::RequestRespawn_Implementation()
{
	ATPSGameMode* mode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(this));
	if (IsValid(mode))
	{
		UE_LOG(LogTemp, Log, TEXT("Requesting RESPAWN for PlayerController[%s]..."), *GetName());
		mode->RequestRespawn(this);
	}
}

void ATPSPlayerController::NotifyPawnDeath()
{
	RequestRespawn();
}



//~ ====================================================================== ~//
//- CONSOLE COMMANDS AND CONFIGURATION
//~ ====================================================================== ~//

static TAutoConsoleVariable<int32> CVarLocalPlayerDebugMode(
	TEXT("TPS.LocalPlayerDebugMode"),
	0,
	TEXT("Shows debug mode for local player.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));
static TAutoConsoleVariable<int32> CVarLocalPlayerCrouchToggleMode(
	TEXT("TPS.LocalPlayerGodMode"),
	0,
	TEXT("Enables GOD MODE for local player.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));
static TAutoConsoleVariable<int32> CVarLocalPlayerGodMode(
	TEXT("TPS.LocalPlayerGodMode"),
	0,
	TEXT("Enables GOD MODE for local player.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));

void _OnControllerConfiugrationConsoleInput(IConsoleVariable* Var) {
	UE_LOG(LogTemp, Log, TEXT("Performing CONFIG Update..."));

	FTPSControllerConfiguration configuration = {
		CVarLocalPlayerDebugMode->GetInt(),
		CVarLocalPlayerGodMode->GetBool(),
		CVarLocalPlayerCrouchToggleMode->GetBool()
	};
	UE_LOG(LogTemp, Log, TEXT("INPUT CharacterDEBUG: %i\nINPUT GodMode: %b"),
		configuration.LocalCharacterDebugMode, configuration.GodModeEnabled);
	ATPSPlayerController::UpdateControllerConfiguration(&configuration);
}
void ATPSPlayerController::BindConsoleCallbacks() {
	CVarLocalPlayerDebugMode.AsVariable()
		->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda(&_OnControllerConfiugrationConsoleInput));
	CVarLocalPlayerCrouchToggleMode.AsVariable()
		->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&_OnControllerConfiugrationConsoleInput));
	CVarLocalPlayerGodMode.AsVariable()
		->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&_OnControllerConfiugrationConsoleInput));

	_OnControllerConfiugrationConsoleInput(nullptr); // <- perform initial read (hacky)
}
FTPSControllerConfiguration* ATPSPlayerController::configuration = new FTPSControllerConfiguration();
void ATPSPlayerController::UpdateControllerConfiguration(FTPSControllerConfiguration* config) {
	configuration->LocalCharacterDebugMode = config->LocalCharacterDebugMode;
	configuration->GodModeEnabled = config->GodModeEnabled;
}


//~ ====================================================================== ~//
//- TPSGameState Overrides
//~ ====================================================================== ~//

void ATPSPlayerController::TPS_OverrideCurrentCharacterStateFromString(FString input) {
	ETPSCharacterBehaviorState newState = ETPSCharacterStateFromString(TCHAR_TO_ANSI(*input));
	UE_LOG(LogTemp, Log, TEXT("DEBUG OVERRIDE: CharacterState -> [%s]-[%i]"), TCHAR_TO_ANSI(*input), newState);
	OnCharacterStateOverride(newState);
}
void ATPSPlayerController::TPS_OverrideCurrentCharacterStateFromInt(int32 input) {
	ETPSCharacterBehaviorState newState = ETPSCharacterBehaviorState(input);
	UE_LOG(LogTemp, Log, TEXT("DEBUG OVERRIDE: CharacterState -> [%i]-[%i]"), input, newState);
	OnCharacterStateOverride(newState);
}
void ATPSPlayerController::Kill() {
	OnCharacterStateOverride(Incapacitated);
}
void ATPSPlayerController::Injure() {
	OnCharacterStateOverride(Injured);
}
void ATPSPlayerController::EnterCombat() {
	OnCharacterStateOverride(Combat);
}
void ATPSPlayerController::ExitCombat() {
	OnCharacterStateOverride(Casual);
}

void ATPSPlayerController::TPS_ToggleDebugForLocalPlayer() {	
	IsDebugEnabled = (!IsDebugEnabled);
	configuration->LocalCharacterDebugMode = (configuration->LocalCharacterDebugMode == 1)
		? 0
		: 1;
}
void ATPSPlayerController::DebugLocal() {
	TPS_ToggleDebugForLocalPlayer();
}


void ATPSPlayerController::TPS_ToggleCrouchForLocalPlayer() {
	IsDebugEnabled = (!IsDebugEnabled);
	configuration->crouchToggle = (configuration->crouchToggle == 1)
		? 0
		: 1;
}
void ATPSPlayerController::ToggleCrouch() {
	TPS_ToggleCrouchForLocalPlayer();
}


//~ ====================================================================== ~//
//- Pawn Possession
//~ ====================================================================== ~//

//~ RESPAWN ~//

void ATPSPlayerController::Respawn()
{
	RequestRespawn();
}

//~ POSSESS ~//

// AController::OnPossess()
void ATPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnPawnPossessionChanged();
}



void ATPSPlayerController::PossessNearestPlayablePawn_Implementation()
{
	TArray<AActor*> exclusionList = TArray<AActor*>();
	if (IsValid(GetPawn()))
	{
		exclusionList.Add(GetPawn());
	}

	AActor* c = UTPSFunctionLibrary::GetNearestPlayableCharacter(
		this,
						GetTransformComponent()->GetRelativeLocation(), 10000,
						exclusionList);

	ATPSCharacter* character = Cast<ATPSCharacter>(c);
	if (!IsValid(character))
	{
		UE_LOG(LogTemp, Log, TEXT("No available characters found for possession."));
		return;
	}

	ATPSGameMode* mode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(this));
	if (IsValid(mode)) {
		UE_LOG(LogTemp, Log, TEXT("Requesting POSSESS for PlayerController[%s]-Character[%s]..."),
		*GetName(), *character->GetName());
		mode->RequestPossessCharacterActor(this, character);
	}
}
void ATPSPlayerController::PossessPawn()
{
	PossessNearestPlayablePawn();
}

//~ UN-POSSESS ~//

// AController::OnUnPossess()
void ATPSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	OnPawnPossessionChanged();
}

void ATPSPlayerController::UnPossessCurrentPawn_Implementation()
{
	UnPossess();
}
void ATPSPlayerController::UnPossessPawn() {
	UnPossessCurrentPawn();
}
