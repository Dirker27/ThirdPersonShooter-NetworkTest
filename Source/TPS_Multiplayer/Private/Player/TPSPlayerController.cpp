// (C) ToasterCat Studios 2025


#include "Player/TPSPlayerController.h"

#include "Game/TPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Util/TPSFunctionLibrary.h"

ATPSPlayerController::ATPSPlayerController()
{
	//bHidden = false;
#if WITH_EDITORONLY_DATA
	//bHiddenEd = false;
#endif // WITH_EDITORONLY_DATA
	//SetHidden(false);

	ControllerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ControllerCamera"));
}


void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


//~ ====================================================================== ~//
//- BEHAVIOR OPERATIONS
//~ ====================================================================== ~//

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
	ETPSCharacterState newState = ETPSCharacterStateFromString(TCHAR_TO_ANSI(*input));
	UE_LOG(LogTemp, Log, TEXT("DEBUG OVERRIDE: CharacterState -> [%s]-[%i]"), TCHAR_TO_ANSI(*input), newState);
	OnCharacterStateOverride(newState);
}
void ATPSPlayerController::TPS_OverrideCurrentCharacterStateFromInt(int32 input) {
	ETPSCharacterState newState = ETPSCharacterState(input);
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

void ATPSPlayerController::UnPossessCurrentPawn_Implementation()
{
	UnPossess();
}
void ATPSPlayerController::UnPossessPawn() {
	UnPossessCurrentPawn();
}
