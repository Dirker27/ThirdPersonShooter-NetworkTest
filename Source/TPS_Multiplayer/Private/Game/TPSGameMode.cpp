// (C) ToasterCat Studios 2025

#include "Game/TPSGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Game/TPSGameState.h"
#include "Team/TPSTeamInstanceFactory.h"
#include "Types/TPSReport.h"
#include "World/Spawn/TPSSpawnPoint.h"
#include "World/TPSWorldManager.h"

//~ ====================================================================== ~//
//  CONSTRUCTORS
//~ ====================================================================== ~//

ATPSGameMode::ATPSGameMode()
{
	WorldManager = CreateDefaultSubobject<UTPSWorldManager>(TEXT("WorldState"));
	TeamInstanceFactory = CreateDefaultSubobject<UTPSTeamInstanceFactory>(TEXT("TeamInstanceFactory"));
	CharacterInstanceFactory = CreateDefaultSubobject<UTPSCharacterInstanceFactory>(TEXT("CharacterInstanceFactory"));
	CombatLog = CreateDefaultSubobject<UTPSCombatLog>(TEXT("CombatLog"));
}
ATPSGameMode::~ATPSGameMode() { }

void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::BeginPlay()"));
}


//~ ====================================================================== ~//
//  OPERATIONS
//~ ====================================================================== ~//

void ATPSGameMode::StartMatch()
{
	Super::StartMatch();
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::StartMatch()"));

	State()->MatchTimeLimitSeconds = TimeLimitSeconds;
	State()->TimeMatchStarted = UGameplayStatics::GetTimeSeconds(this);
}

void ATPSGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::HandleMatchHasStarted()"));
}


void ATPSGameMode::EndMatch()
{
	Super::EndMatch();
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::EndMatch()"));
}

void ATPSGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::HandleMatchHasEnded()"));
}


void ATPSGameMode::BroadcastMessage(FTPSBroadcastMessage message)
{
	State()->UpdateBroadcastMessage(message);

	CombatLog->LogMessage(FTPSLogMessage(message.Heading, message.SubHeading));
}


void ATPSGameMode::UpdateMatchPhase(ETPSMatchPhase phase)
{
	State()->UpdateMatchPhase(phase);
}



void ATPSGameMode::IndexSpawnPoints()
{
	ATPSGameState* state = GetGameState<ATPSGameState>();

	// ALL SpawnPoints -> TEAM
	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, ATPSSpawnPoint::StaticClass(), spawnPoints);
	for (auto sp : spawnPoints)
	{
		if (ATPSSpawnPoint* spawn = Cast<ATPSSpawnPoint>(sp))
		{
			if (UTPSTeamInstance* t = state->GetTeam(spawn->AssignedTeamID))
			{
				t->SpawnPool->AddSpawnPointToPool(spawn);
			}
		}
	}

	// ALL SpawnFormations -> TEAM
	TArray<AActor*> spawnFormations;
	UGameplayStatics::GetAllActorsOfClass(this, ATPSSpawnFormation::StaticClass(), spawnFormations);
	for (auto sp : spawnPoints)
	{
		if (ATPSSpawnFormation* formation = Cast<ATPSSpawnFormation>(sp))
		{
			if (UTPSTeamInstance* t = state->GetTeam(formation->AssignedTeamID))
			{
				t->SpawnPool->AddSpawnFormationToPool(formation);
			}
		}
	}
}


void ATPSGameMode::SpawnTeam(ETPSTeamID teamId)
{
	if (UTPSTeamInstance* t = State()->GetTeam(teamId))
	{
		for (auto army : t->Armies) {
			_SpawnUnit(army->GetRootUnit(), t->SpawnPool);
		}
	}
}

void ATPSGameMode::SpawnUnit(FTPSUnitID unitId)
{
	ATPSGameState* state = State();

	if (UTPSCommandUnit* unit = state->GetUnit(unitId))
	{
		if (auto team = unit->GetAssignedTeam()) {
			_SpawnUnit(unit, team->SpawnPool);
		}
	}
}

void ATPSGameMode::_SpawnUnit(UTPSCommandUnit* unit, UTPSSpawnPool* spawnPool)
{
	for (auto instance : unit->GetAllMembers())
	{
		AActor* spawn = spawnPool->FindBestSpawnPointForUnitAndSquadRole(
			instance->GetAssignedUnit(),
			instance->Identity.SquadRole);

		instance->SpawnActor(PlayerCharacterTemplate, spawn);
	}

	for (auto subUnit : unit->GetAllSubCollections())
	{
		_SpawnUnit(Cast<UTPSCommandUnit>(subUnit), spawnPool);
	}
}



void ATPSGameMode::InitializeTeams()
{
	for (auto teamConfig : TeamDefinitionMap)
	{
		TeamInstanceFactory->CreateTeam(teamConfig.Key);
		TeamInstanceFactory->ConfigureTeam(teamConfig.Key, teamConfig.Value->Definition);
	}
}


void ATPSGameMode::PopulateTeams()
{
	TArray<TObjectPtr<UTPSCharacterInstance>> roster;
	for (auto team : State()->Teams)
	{
		TeamInstanceFactory->PopulateTeam(team->TeamID, roster);
	}
}

void ATPSGameMode::SpawnTeams()
{
	TArray<TObjectPtr<UTPSCharacterInstance>> roster;
	for (auto team : State()->Teams)
	{
		SpawnTeam(team->TeamID);
	}
}


void ATPSGameMode::KillCharacter_Implementation(const FTPSCharacterID characterId)
{
	UE_LOG(LogTemp, Log, TEXT("RECEIVED REQUEST::KillCharacter([%s])..."), *characterId.Guid.ToString());
	ATPSGameState* state = GetGameState<ATPSGameState>();

	if (auto character = state->GetCharacter(characterId))
	{
		character->Die();

		auto report = GenerateEliminationReportForCharacterDeath(character);
		CombatLog->LogElimination(report);

		// Broadcast Event -> BP GameMode handler
		OnCharacterElimination(report);
	}
}


FTPSEliminationReport ATPSGameMode::GenerateEliminationReportForCharacterDeath(UTPSCharacterInstance* eliminatee) const
{
	UTPSCharacterInstance* killerInstance = nullptr;
	FString killMethod = "Divine Intervention";

	if (auto killerActor = Cast<ATPSCharacter>(eliminatee->LastHit.Instigator))
	{
		killerInstance = killerActor->GetCharacterInstance();
		if (auto killWeapon = killerActor->GetEquippedWeapon())
		{
			killMethod = killWeapon->Name;
		}
	}

	return FTPSEliminationReport(
		eliminatee, 
		killerInstance, 
		killMethod,
		UGameplayStatics::GetTimeSeconds(this));
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

	if (UTPSTeamInstance* team = instance->GetAssignedTeam())
	{
		return team->SpawnPool->FindBestSpawnPointForSquadRole(instance->Identity.SquadRole);
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
