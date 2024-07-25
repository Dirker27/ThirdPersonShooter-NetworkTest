// (C) ToasterCat Studios 2024


#include "TPSPlayerController.h"

static TAutoConsoleVariable<int32> CVarLocalPlayerDebugMode(
	TEXT("TPS.LocalPlayerDebugMode"),
	0,
	TEXT("Shows debug mode for local player.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));

static TAutoConsoleVariable<int32> CVarLocalCharacterStateOverride(
	TEXT("TPS.LocalCharacterStateOverride"),
	0,
	TEXT("Overrides current character state\n")
	TEXT("<=0: Casual\n")
	TEXT("  1: Combat\n")
	TEXT("  2: Injured\n")
	TEXT("  3: Incapacitated"));

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

void ATPSPlayerController::OverrideCurrentCharacterState() {
	UE_LOG(LogTemp, Display, TEXT("Current override state is %i"), CVarLocalCharacterStateOverride.GetValueOnGameThread());
}

void ATPSPlayerController::ShowDebugForLocalPlayer() {
	UE_LOG(LogTemp, Error, TEXT("DebugForLocalPlayer: %i"), CVarLocalPlayerDebugMode.GetValueOnGameThread());
}

void ATPSPlayerController::OnDebugInputDelegate(IConsoleVariable* Var) {
	UE_LOG(LogTemp, Log, TEXT("-DEBUG INPUT-"));
	OnDebugInput();
}