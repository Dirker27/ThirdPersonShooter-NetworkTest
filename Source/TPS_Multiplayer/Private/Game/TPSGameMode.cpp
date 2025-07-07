// (C) ToasterCat Studios 2024

#include "Game/TPSGameMode.h"

#include "Kismet/GameplayStatics.h"

//~ ====================================================================== ~//
//  CONSTRUCTORS
//~ ====================================================================== ~//

ATPSGameMode::ATPSGameMode()
{
	TeamManager = CreateDefaultSubobject<UTPSTeamManager>(TEXT("TeamManager"));
	WorldManager = CreateDefaultSubobject<UTPSWorldManager>(TEXT("WorldManager"));
}
ATPSGameMode::~ATPSGameMode() { }

void ATPSGameMode::BeginPlay()
{
	InitializeTeams();
}


//~ ====================================================================== ~//
//  Operations
//~ ====================================================================== ~//

void ATPSGameMode::IndexSpawnPointsForTeams()
{
	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, ATPSSPawnPoint::StaticClass(), spawnPoints);

	for (auto sp : spawnPoints)
	{
		if (ATPSSPawnPoint* spawn = Cast<ATPSSPawnPoint>(sp))
		{
			if (UTPSTeam* t = TeamManager->GetTeam(spawn->TeamID))
			{
				t->SpawnPoints.Add(spawn);
			}
		}
	}
}


void ATPSGameMode::InitializeTeams()
{
	TeamManager->CreateTeam(ETPSTeamID::Red);
	TeamManager->ConfigureTeam(ETPSTeamID::Red, 1, 4);

	TeamManager->CreateTeam(ETPSTeamID::Blue);
	TeamManager->ConfigureTeam(ETPSTeamID::Blue, 1, 3);
}


void ATPSGameMode::SpawnRedTeam()
{
	
}

void ATPSGameMode::SpawnBlueTeam()
{
	
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

	// Should perform from PlayerController?
	//playerController->Possess(p);

	//return p;
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
		APlayerStart* spawnPoint = FindSpawnPoint();
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
		APlayerStart* spawnPoint = FindSpawnPoint();
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

APlayerStart* ATPSGameMode::FindSpawnPoint()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActors);

	int32 RandomIndex = FMath::RandRange(0, OutActors.Num() - 1);

	// Return the element at the random index
	return Cast<APlayerStart>(OutActors[RandomIndex]);
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
