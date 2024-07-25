// (C) ToasterCat Studios 2024


#include "TPSGameMode.h"

static TAutoConsoleVariable<int32> CVarGlobalPlayerDebugMode(
	TEXT("TPS.GlobalPlayerDebugMode"),
	0,
	TEXT("Shows debug mode for all connected player controllers.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));

static TAutoConsoleVariable<int32> CVarGlobalCharacterDebugMode(
	TEXT("TPS.GlobalCharacterDebugMode"),
	0,
	TEXT("Shows debug mode for all active characters.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));

void ATPSGameMode::ShowDebugForAll() {
	UE_LOG(LogTemp, Log, TEXT("Debug Mode for ALL Players: %i"), CVarGlobalPlayerDebugMode.GetValueOnGameThread());
	UE_LOG(LogTemp, Log, TEXT("Debug Mode for ALL Characters: %i"), CVarGlobalCharacterDebugMode.GetValueOnGameThread());
}


bool ATPSGameMode::RequestRespawn() {
	return true;
}

void ATPSGameMode::PerformRespawn(ATPSPlayerController playerController) {
	// TODO
}
