// (C) ToasterCat Studios 2024


#include "TPSPlayerController.h"

static TAutoConsoleVariable<int32> CVarLocalPlayerDebugMode(
	TEXT("TPS.LocalPlayerDebugMode"),
	0,
	TEXT("Shows debug mode for local player.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));

static void _OnDebugStateChange(IConsoleVariable* Var)
{
	UE_LOG(LogTemp, Log, TEXT("VAR CHANGE: %i"), Var->GetInt());
}

void ATPSPlayerController::BindConsoleCallbacks() {
	CVarLocalPlayerDebugMode.AsVariable()
		->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&_OnDebugStateChange));

	//CVarLocalPlayerDebugMode.AsVariable()
	//	->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda(&OnDebugInput));
}

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

void ATPSPlayerController::TPS_ShowDebugForLocalPlayer() {
	UE_LOG(LogTemp, Log, TEXT("DebugForLocalPlayer: %i"), CVarLocalPlayerDebugMode.GetValueOnGameThread());
}

void ATPSPlayerController::OnDebugInputDelegate(IConsoleVariable* Var) {
	UE_LOG(LogTemp, Log, TEXT("-DEBUG INPUT-: %i"), Var->GetInt());
	OnDebugInput();
}