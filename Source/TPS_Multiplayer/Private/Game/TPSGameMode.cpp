// (C) ToasterCat Studios 2024

#include "Game/TPSGameMode.h"

#include "Character/TPSPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

//~ ====================================================================== ~//
//- CONSOLE CONFIGURATION
//~ ====================================================================== ~//

static TAutoConsoleVariable<int32> CVarGlobalCharacterDebugMode(
	TEXT("TPS.GlobalCharacterDebugMode"),
	0,
	TEXT("Shows debug mode for all active characters.\n")
	TEXT("<=0: OFF\n")
	TEXT("  1: ON\n"));
static TAutoConsoleVariable<int32> CVarGlobalFogDensity(
	TEXT("TPS.GlobalFogDensity"),
	0,
	TEXT("How dense fog should be for the shared map environment.\n")
	TEXT("<=0.0: Transparent\n")
	TEXT("  0.2: Typical\n")
	TEXT("  1.0: Opaque\n"));


void _OnGameConfiugrationConsoleInput(IConsoleVariable* Var) {
	UE_LOG(LogTemp, Log, TEXT("Performing CONFIG Update..."));

	FTPSGameConfiguration configuration = {
		CVarGlobalCharacterDebugMode->GetInt(),
		CVarGlobalFogDensity->GetFloat()
	};
	UE_LOG(LogTemp, Log, TEXT("INPUT CharacterDEBUG: %i\nINPUT FogDensity: %f"),
		configuration.GlobalCharacterDebugMode, configuration.GlobalFogDensity);
	ATPSGameMode::UpdateGameConfiguration(&configuration);
}
void ATPSGameMode::BindConsoleCallbacks() {
	CVarGlobalFogDensity.AsVariable()
		->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&_OnGameConfiugrationConsoleInput));
	CVarGlobalCharacterDebugMode.AsVariable()
		->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&_OnGameConfiugrationConsoleInput));

	_OnGameConfiugrationConsoleInput(nullptr); // <- perform initial read (hacky)
}
FTPSGameConfiguration* ATPSGameMode::Configuration = new FTPSGameConfiguration(); //{ 0, 0.2f };
void ATPSGameMode::UpdateGameConfiguration(FTPSGameConfiguration* config) {
	Configuration->GlobalCharacterDebugMode = config->GlobalCharacterDebugMode;
	Configuration->GlobalFogDensity = config->GlobalFogDensity;
}


//~ ====================================================================== ~//
//- CONSTRUCTORS
//~ ====================================================================== ~//

ATPSGameMode::ATPSGameMode() { }
ATPSGameMode::~ATPSGameMode() { }


//~ ====================================================================== ~//
//- Operations
//~ ====================================================================== ~//

bool ATPSGameMode::RequestRespawn(ATPSPlayerController* playercontroller) {
	return true;
}

void ATPSGameMode::PerformRespawn(ATPSPlayerController* playerController) {
	ATPSCharacter* p = SpawnPlayer();

	playerController->Possess(p);
}


//~ ====================================================================== ~//
//- Utilities
//~ ====================================================================== ~//

void ATPSGameMode::TPS_ToggleDebugForAllCharacters() {
	IsDebugEnabled = (!IsDebugEnabled);
	Configuration->GlobalCharacterDebugMode = (Configuration->GlobalCharacterDebugMode == 1)
		? 0
		: 1;
}
void ATPSGameMode::DebugGlobal() {
	TPS_ToggleDebugForAllCharacters();
}


ATPSCharacter* ATPSGameMode::TPS_SpawnNewPlayerCharacter()
{
	ATPSCharacter* spawnedCharacter = nullptr;
	if (IsValid(PlayerCharacterTemplate))
	{
		APlayerStart* spawnPoint = FindSpawnPoint();
		if (IsValid(spawnPoint))
		{
			spawnedCharacter = GetWorld()->SpawnActor<ATPSPlayerCharacter>(PlayerCharacterTemplate,
				spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());
		}
		else
		{
			spawnedCharacter = GetWorld()->SpawnActor<ATPSPlayerCharacter>(PlayerCharacterTemplate);
		}
	}

	return spawnedCharacter;
}
ATPSCharacter* ATPSGameMode::SpawnPlayer() {
	return TPS_SpawnNewPlayerCharacter();
}
void ATPSGameMode::SpawnPlayers(int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		SpawnPlayer();
	}
}

ATPSCharacter* ATPSGameMode::TPS_SpawnNewBot()
{
	ATPSCharacter* spawnedBot = nullptr;
	if (IsValid(BotTemplate))
	{
		APlayerStart* spawnPoint = FindSpawnPoint();
		if (IsValid(spawnPoint))
		{
			spawnedBot = GetWorld()->SpawnActor<ATPSAICharacter>(BotTemplate,
				spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());
		}
		else
		{
			spawnedBot = GetWorld()->SpawnActor<ATPSAICharacter>(BotTemplate);
		}
	}

	return spawnedBot;
}
ATPSCharacter* ATPSGameMode::SpawnBot() {
	return TPS_SpawnNewBot();
}
void ATPSGameMode::SpawnBots(int numBots)
{
	for (int i = 0; i < numBots; i++)
	{
		SpawnBot();
	}
}

APlayerStart* ATPSGameMode::FindSpawnPoint()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActors);

	int32 RandomIndex = FMath::RandRange(0, OutActors.Num() - 1);

	// Return the element at the random index
	return Cast<APlayerStart>(OutActors[RandomIndex]);
}