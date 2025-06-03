// (C) ToasterCat Studios 2025


#include "Player/TPSPlayerController.h"

#include "Game/TPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
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


void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::BeginPlay()"));
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::SetupInputComponent()"));
}

void ATPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Log, TEXT("TPSPlayerController::OnRep_PlayerState()"));
}

// PlayerControllers are NOT replicated to peer clients
//   This sync will only ever be between Server<->OwningClient
void ATPSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSPlayerController, PossessedCharacter);
	DOREPLIFETIME(ATPSPlayerController, PossessedPawn);
}

void ATPSPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PossessedCharacter)
	{
		PossessedCharacter->SetTargetLocation(GetCameraTargetLocation());
	}
	if (PossessedPawn)
	{
		PossessedPawn->SetTargetLocation(GetCameraTargetLocation());
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
//- State Overrides
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

	if (ATPSCharacter* c = Cast<ATPSCharacter>(InPawn))
	{
		PossessedCharacter = c;
	}
	else if (ATPSPawn* p = Cast<ATPSPawn>(InPawn))
	{
		PossessedPawn = p;
	}
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
		mode->RequestPossession(this, character);
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
	PossessedCharacter = nullptr;
	PossessedPawn = nullptr;
}

void ATPSPlayerController::UnPossessCurrentPawn_Implementation()
{
	UnPossess();
}
void ATPSPlayerController::UnPossessPawn() {
	UnPossessCurrentPawn();
}
