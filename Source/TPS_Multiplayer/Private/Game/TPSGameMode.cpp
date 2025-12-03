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
	CombatLog = CreateDefaultSubobject<UTPSCombatLog>(TEXT("CombatLog"));

	TeamFactory = CreateDefaultSubobject<UTPSTeamInstanceFactory>(TEXT("TeamFactory"));
	CharacterFactory = CreateDefaultSubobject<UTPSCharacterInstanceFactory>(TEXT("CharacterFactory"));
}
ATPSGameMode::~ATPSGameMode() { }

void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::BeginPlay()"));

	if (IsValid(TeamFactory)) {
		TeamFactory->ArmyFactory = ArmyFactory;
		TeamFactory->UnitFactory = UnitFactory;
		TeamFactory->CharacterFactory = CharacterFactory;
	} else {
		UE_LOG(LogTemp, Log, TEXT("[GAME MODE] NO INSTANCE FACTORY PROVIDED!!!"));
	}
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


void ATPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::PostLogin()"));

	if (auto ps = NewPlayer->GetPlayerState<ATPSPlayerState>())
	{
		ps->ID.Guid = FGuid::NewGuid();
		ps->ID.UEPlayerID = ps->GetPlayerId();
		UE_LOG(LogTemp, Log, TEXT("Player[%s]-[%i] logged in."),
			*ps->ID.ToString(), ps->GetPlayerId());
	}

	/*if (ATPSPlayerState* ps = NewPlayer->GetPlayerState<ATPSPlayerState>())
	{
		ps->AssignToTeam(GetRandomTeam());
	}*/
}

UTPSTeamInstance* ATPSGameMode::GetRandomTeam() const
{
	if (State()->Teams.IsEmpty()) { return nullptr; }

	int max = State()->Teams.Num() - 1;
	int i = FMath::RandRange(0, max);
	return State()->Teams[i];
}




void ATPSGameMode::BroadcastMessage(FTPSBroadcastMessage message)
{
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::BroadcastMessage([%s]-[%s])"),
		*message.Heading, *message.SubHeading);

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





bool ATPSGameMode::SpawnCharacter(FTPSCharacterID characterId)
{
	UE_LOG(LogTemp, Log, TEXT("Spawning Character[%s]..."), *characterId.ToString());

	if (UTPSCharacterInstance* character = State()->GetCharacter(characterId))
	{
		if (auto team = character->GetAssignedTeam()) {
			character->SpawnActor(DefaultCharacterTemplate, FindSpawnPointForCharacter(character));
			UE_LOG(LogTemp, Log, TEXT("Character[%s] spawned."), *characterId.ToString());
			return true;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("FAILED to spawn Character[%s]!"), *characterId.ToString());
	return false;
}

void ATPSGameMode::SpawnTeams()
{
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::InitializeTeams()"));

	TArray<TObjectPtr<UTPSCharacterInstance>> roster;
	for (auto team : State()->Teams)
	{
		SpawnTeam(team->TeamID);
	}
}

bool ATPSGameMode::SpawnTeam(ETPSTeamID teamId)
{
	UE_LOG(LogTemp, Log, TEXT("TPSGameMode::SpawnTeam([%s])"), *TPSTeamIdToString(teamId));

	bool success = true;
	if (UTPSTeamInstance* t = State()->GetTeam(teamId))
	{
		for (auto army : t->GetArmies()) {
			success &= SpawnArmy(army->ArmyID);
		}
	}
	return success;
}

bool ATPSGameMode::SpawnArmy(FTPSArmyID armyId)
{
	UE_LOG(LogTemp, Log, TEXT("Spawning Army[%s]..."), *armyId.ToString());
	if (UTPSArmyInstance* army = State()->GetArmy(armyId))
	{
		if (auto unit = army->GetRootUnit()) {
			SpawnUnit(army->GetRootUnit()->UnitID);
			UE_LOG(LogTemp, Log, TEXT("Army[%s] spawned."), *armyId.ToString());
			return true;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("FAILED to spawn Army[%s]!"), *armyId.ToString());
	return false;
}

bool ATPSGameMode::SpawnUnit(FTPSUnitID unitId)
{
	UE_LOG(LogTemp, Log, TEXT("Spawning Unit[%s]..."), *unitId.ToString());
	if (UTPSCommandUnit* unit = State()->GetUnit(unitId))
	{
		if (auto team = unit->GetAssignedTeam()) {
			_SpawnUnit(unit, team->SpawnPool);
			UE_LOG(LogTemp, Log, TEXT("Unit[%s] spawned."), *unitId.ToString());
			return true;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("FAILED to spawn Unit[%s]!"), *unitId.ToString());
	return false;
}

void ATPSGameMode::_SpawnUnit(UTPSCommandUnit* unit, UTPSSpawnPool* spawnPool)
{
	for (auto instance : unit->GetAllMembers())
	{
		AActor* spawn = spawnPool->FindBestSpawnPointForUnitAndSquadRole(
			instance->GetAssignedUnit(),
			instance->Identity.SquadRole);

		instance->SpawnActor(DefaultCharacterTemplate, spawn);
	}

	for (auto subUnit : unit->GetAllSubCollections())
	{
		_SpawnUnit(Cast<UTPSCommandUnit>(subUnit), spawnPool);
	}
}



void ATPSGameMode::AssignToArmyAndPossess_Implementation(ATPSPlayerController* controller, UTPSArmyInstance* army)
{
	ATPSPlayerState* player = controller->GetPlayerState<ATPSPlayerState>();

	AssignPlayerToArmy(player, army);

	if (auto team = army->GetAssignedTeam()) {
		AssignPlayerToTeam(player, team);
	}

	if (auto unit = army->GetRootUnit())
	{
		RequestPossessUnitLeader_Implementation(controller, unit);
	}
}


void ATPSGameMode::AssignPlayerToArmy(ATPSPlayerState* player, UTPSArmyInstance* army) const
{
	player->AssignToArmy(army);
	army->BindToPlayer(player);
}

void ATPSGameMode::AssignPlayerToTeam(ATPSPlayerState* player, UTPSTeamInstance* team) const
{
	player->AssignToTeam(team);
	team->AddPlayer(player);
}







void ATPSGameMode::InitializeTeams()
{
	UE_LOG(LogTemp, Log, TEXT("[GameMode] Initializing Teams..."));
	for (auto teamConfig : TeamDefinitionMap)
	{
		TeamFactory->CreateTeam(teamConfig.Key);
		TeamFactory->ConfigureTeam(teamConfig.Key, teamConfig.Value->Definition);
	}
	UE_LOG(LogTemp, Log, TEXT("[GameMode] Teams Initialized."));
}


void ATPSGameMode::PopulateTeams()
{
	UE_LOG(LogTemp, Log, TEXT("[GameMode] Populating Teams..."));
	TArray<TObjectPtr<UTPSCharacterInstance>> roster;
	for (auto team : State()->Teams)
	{
		TeamFactory->PopulateTeam(team->TeamID, roster);
	}
	UE_LOG(LogTemp, Log, TEXT("[GameMode] Teams Populated."));
}

void ATPSGameMode::KillCharacter_Implementation(const FTPSCharacterID characterId)
{
	UE_LOG(LogTemp, Log, TEXT("RECEIVED REQUEST - TPSGameMode::KillCharacter([%s])..."), *characterId.Guid.ToString());

	if (auto character = State()->GetCharacter(characterId))
	{
		UE_LOG(LogTemp, Log, TEXT("Eliminating Character[%s]..."), *characterId.ToString());
		character->Die();

		auto report = GenerateEliminationReportForCharacterDeath(character);
		CombatLog->LogElimination(report);

		// Broadcast Event -> BP GameMode handler
		OnCharacterElimination(report);
		UE_LOG(LogTemp, Log, TEXT("Character[%s] Eliminated."), *characterId.ToString());
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

void ATPSGameMode::RequestPossessCharacterActor_Implementation(ATPSPlayerController* controller, ATPSCharacter* character)
{
	if (IsValid(controller) && IsValid(character) && character->CanBePossessedByPlayer)
	{
		controller->Possess(character);
	}
}

void ATPSGameMode::RequestPossessCharacterInstance_Implementation(ATPSPlayerController* controller, UTPSCharacterInstance* character)
{
	if (!IsValid(controller) || !IsValid(character)) { return; }

	if (ATPSPlayerState* player = controller->GetPlayerState<ATPSPlayerState>()) {
		if (CanCharacterBePossessedByPlayer(player, character))
		{
			controller->Possess(character->GetSpawnedActor());
		}
	}
}


void ATPSGameMode::RequestPossessUnitLeader_Implementation(ATPSPlayerController* controller, UTPSCommandUnit* unit)
{
	if (!IsValid(controller) || !IsValid(unit)) { return; }

	if (auto player = controller->GetPlayerState<ATPSPlayerState>()) {
		if (CanCharacterBePossessedByPlayer(player, unit->GetLeader()))
		{
			controller->Possess(unit->GetLeader()->GetSpawnedActor());
		}
	}
}



bool ATPSGameMode::CanCharacterBePossessedByPlayer_Implementation(ATPSPlayerState* player, UTPSCharacterInstance* character) const
{
	if (!IsValid(player) || !IsValid(character)) { return false; }

	return player->GetAssignedTeamID() == character->GetAssignedTeamID()
		&& character->IsAlive
		&& IsValid(character->GetSpawnedActor())
		&& character->GetSpawnedActor()->CanBePossessedByPlayer;
}


//~ Spawn Players ~//

void ATPSGameMode::SpawnNewPlayerCharacter_Implementation(AController* controller)
{
	ATPSCharacter* spawned = nullptr;
	if (IsValid(DefaultCharacterTemplate))
	{
		AActor* spawnPoint = FindPlayerStart(controller, TEXT(""));
		if (IsValid(spawnPoint))
		{
			spawned = GetWorld()->SpawnActor<ATPSCharacter>(DefaultCharacterTemplate,
				spawnPoint->GetTransform().GetLocation(), spawnPoint->GetTransform().Rotator());
		}
		else
		{
			spawned = GetWorld()->SpawnActor<ATPSCharacter>(DefaultCharacterTemplate);
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
