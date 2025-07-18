// (C) ToasterCat Studios 2024

#include "Game/TPSGameMode.h"

#include "Game/TPSGameState.h"
#include "Kismet/GameplayStatics.h"

//~ ====================================================================== ~//
//  CONSTRUCTORS
//~ ====================================================================== ~//

ATPSGameMode::ATPSGameMode()
{
}
ATPSGameMode::~ATPSGameMode() { }

void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
}


//~ ====================================================================== ~//
//  OPERATIONS
//~ ====================================================================== ~//

void ATPSGameMode::IndexSpawnPoints()
{
	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, ATPSSpawnPoint::StaticClass(), spawnPoints);

	ATPSGameState* state = GetGameState<ATPSGameState>();
	for (auto sp : spawnPoints)
	{
		if (ATPSSpawnPoint* spawn = Cast<ATPSSpawnPoint>(sp))
		{
			if (UTPSTeam* t = state->GetTeam(spawn->AssignedUnit.TeamID))
			{
				t->SpawnPool->AddSpawnPointToPool(spawn);
			}
		}
	}
}


void ATPSGameMode::SpawnTeam(ETPSTeamID teamId)
{
	ATPSGameState* state = GetGameState<ATPSGameState>();
	if (UTPSTeam* t = state->GetTeam(teamId))
	{
		_SpawnUnit(t->RootCollection, t->SpawnPool);
	}
}

void ATPSGameMode::SpawnUnit(FTPSUnitID unitId)
{
	ATPSGameState* state = GetGameState<ATPSGameState>();
	UTPSTeam* team = state->GetTeam(unitId.TeamID);
	if (UTPSCommandStructure* unit = state->GetUnit(unitId))
	{
		_SpawnUnit(unit, team->SpawnPool);
	}
}

void ATPSGameMode::_SpawnUnit(UTPSCommandStructure* unit, UTPSSpawnPool* spawnPool)
{
	for (auto instance : unit->GetAllMembers())
	{
		AActor* spawn = spawnPool->FindBestSpawnPointForUnitAndSquadRole(
			instance->Identity->UnitID,
			instance->Identity->SquadRole);

		instance->SpawnActor(PlayerCharacterTemplate, spawn);
	}

	for (auto subUnit : unit->GetAllChildCollections())
	{
		_SpawnUnit(Cast<UTPSCommandStructure>(subUnit), spawnPool);
	}
}



void ATPSGameMode::InitializeTeams()
{
	ATPSGameState* state = GetGameState<ATPSGameState>();

	for (auto teamConfig : TeamConfigurationMap)
	{
		state->CreateTeam(teamConfig.Key);
		state->ConfigureTeam(teamConfig.Key, teamConfig.Value->Configuration);
	}
}






void ATPSGameMode::RequestRespawn_Implementation(ATPSPlayerController* playerController)
{
	if (CanRespawn(playerController)) {
		PerformRespawn(playerController);
	}
}

bool ATPSGameMode::CanRespawn(ATPSPlayerController* playerController)
{
	// TODO: Respawn Rules
	return true;
}

void ATPSGameMode::PerformRespawn(ATPSPlayerController* playerController)
{
	SpawnNewPlayerCharacter(playerController);
}


AActor* ATPSGameMode::FindSpawnPointForCharacter(UTPSCharacterInstance* instance)
{
	if (!IsValid(instance))
	{
		return nullptr;
	}

	ATPSGameState* state = GetGameState<ATPSGameState>();

	if (UTPSTeam* team = state->GetTeam(instance->Identity->UnitID.TeamID))
	{
		return team->SpawnPool->FindBestSpawnPointForSquadRole(instance->Identity->SquadRole);
	}

	return FindPlayerStart(nullptr);
}

AActor* ATPSGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	/*TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActors);

	int32 RandomIndex = FMath::RandRange(0, OutActors.Num() - 1);

	// Return the element at the random index
	return Cast<APlayerStart>(OutActors[RandomIndex]);*/

	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}

AActor* ATPSGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}






//~ ====================================================================== ~//
//  Utilities
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

void ATPSGameMode::RequestPossession_Implementation(ATPSPlayerController* controller, ATPSCharacter* character)
{
	if (IsValid(controller) && IsValid(character) && character->CanBePossessedByPlayer)
	{
		controller->Possess(character);
	}
}


//~ Spawn Players ~//

void ATPSGameMode::SpawnNewPlayerCharacter_Implementation(AController* controller)
{
	ATPSCharacter* spawned = nullptr;
	if (IsValid(PlayerCharacterTemplate))
	{
		AActor* spawnPoint = FindPlayerStart(controller, TEXT(""));
		if (IsValid(spawnPoint))
		{
			spawned = GetWorld()->SpawnActor<ATPSCharacter>(PlayerCharacterTemplate,
				spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());
		}
		else
		{
			spawned = GetWorld()->SpawnActor<ATPSCharacter>(PlayerCharacterTemplate);
		}
	}

	if (IsValid(controller))
	{
		controller->Possess(spawned);
	}
}
void ATPSGameMode::SpawnPlayer() {
	SpawnNewPlayerCharacter(nullptr);
}
void ATPSGameMode::SpawnPlayers(int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		SpawnPlayer();
	}
}

//~ Spawn Bots ~//

void ATPSGameMode::SpawnNewBot_Implementation(AController* controller)
{
	ATPSCharacter* spawned = nullptr;
	if (IsValid(BotTemplate))
	{
		AActor* spawnPoint = FindPlayerStart(controller, TEXT(""));
		if (IsValid(spawnPoint))
		{
			spawned = GetWorld()->SpawnActor<ATPSCharacter>(BotTemplate,
				spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());
		}
		else
		{
			spawned = GetWorld()->SpawnActor<ATPSCharacter>(BotTemplate);
		}
	}

	if (IsValid(controller))
	{
		controller->Possess(spawned);
	}
}
void ATPSGameMode::SpawnBot() {
	SpawnNewBot(nullptr);
}
void ATPSGameMode::SpawnBots(int numBots)
{
	for (int i = 0; i < numBots; i++)
	{
		SpawnBot();
	}
}







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
