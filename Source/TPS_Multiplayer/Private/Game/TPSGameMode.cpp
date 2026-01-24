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
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::BeginPlay()"));

	if (IsValid(TeamFactory)) {
		TeamFactory->ArmyFactory = ArmyFactory;
		TeamFactory->UnitFactory = UnitFactory;
		TeamFactory->CharacterFactory = CharacterFactory;
	} else {
		UE_LOG(LogGameMode, Error, TEXT("[SERVER]-[GameMode] NO INSTANCE FACTORY PROVIDED!!!"));
	}
}


//~ ====================================================================== ~//
//  OPERATIONS
//~ ====================================================================== ~//

void ATPSGameMode::StartMatch()
{
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::StartMatch()"));
	Super::StartMatch();

	State()->MatchTimeLimitSeconds = TimeLimitSeconds;
	State()->TimeMatchStarted = UGameplayStatics::GetTimeSeconds(this);
}

void ATPSGameMode::HandleMatchHasStarted()
{
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::HandleMatchHasStarted()"));

	Super::HandleMatchHasStarted();
}


void ATPSGameMode::EndMatch()
{
	Super::EndMatch();
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] TPSGameMode::EndMatch()"));
}

void ATPSGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] TPSGameMode::HandleMatchHasEnded()"));
}


void ATPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::PostLogin()"));
	Super::PostLogin(NewPlayer);

	if (auto ps = NewPlayer->GetPlayerState<ATPSPlayerState>())
	{
		State()->RegisterPlayer(ps);

		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Player[%s]-[%i] logged in."),
			*ps->ID.ToString(), ps->GetPlayerId());
	}
}

void ATPSGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::Logout()"));
	Super::Logout(Exiting);

	if (auto pc = Cast<ATPSPlayerController>(Exiting))
	{
		if (auto ps = pc->BoundPlayer()) {
			State()->UnRegisterPlayer(ps->ID);
		}
	}
}




APawn* ATPSGameMode::GetActivePawnForPlayerSpawn(APlayerController* uePlayerController) const
{
	auto pc = Cast<ATPSPlayerController>(uePlayerController);
	if (!IsValid(pc)) { return nullptr; }

	auto ps = pc->BoundPlayer();
	if (!IsValid(ps)) { return nullptr; }

	if (RequiresPlayerArmy)
	{
		if (auto army = ps->GetAssignedArmy())
		{
			if (auto member = army->GetRandomPossessableMember())
			{
				if (IsValid(member->GetSpawnedActor()))
				{
					return member->GetSpawnedActor();
				}
			}
		}
	}
	else if (RequiresPlayerTeam)
	{
		if (auto team = ps->GetAssignedTeam())
		{
			if(auto member = team->GetRandomPossessableMember())
			{
				if (IsValid(member->GetSpawnedActor()))
				{
					return member->GetSpawnedActor();
				}
			}
		}
	}

	return nullptr;
}



bool ATPSGameMode::PlayerCanRestart_Implementation(APlayerController* uePlayerController)
{
	return uePlayerController->CanRestartPlayer();// && IsValid(GetActivePawnForPlayerSpawn(uePlayerController));
}

void ATPSGameMode::RestartPlayer(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] (Re)Starting Player with Controller[%s]..."), *NewPlayer->GetName());

	ATPSPlayerController* tpc = Cast<ATPSPlayerController>(NewPlayer);
	if (PlayerCanRestart(tpc))
	{
		if (auto pawn = GetActivePawnForPlayerSpawn(tpc))
		{
			UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Player[%s] starting in possession of Pawn[%s]."), *NewPlayer->GetName(), *pawn->GetName());
			tpc->Possess(GetActivePawnForPlayerSpawn(tpc));
			return;
		}
		else
		{
			// TODO: Spawn as DefaultPawn / Spectator
		}
	}


	UE_LOG(LogGameMode, Warning, TEXT("[SERVER]-[GameMode] FAILED to Spawn Player[%s] with instantiated Pawns."), *NewPlayer->GetName());
	Super::RestartPlayer(NewPlayer);
}



void ATPSGameMode::AssignPlayerToAvailableArmy(ATPSPlayerState* player)
{
	if (auto army = GetFirstAvailableArmy())
	{
		player->AssignToArmy(army);
		army->BindToPlayer(player);
		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Player[%s] assigned to Army[%s]"), *player->ID.ToString(), *army->ArmyID.ToString());
	}
}


UTPSArmyInstance* ATPSGameMode::GetFirstAvailableArmy() const
{
	for (auto army : State()->Armies)
	{
		if (!IsValid(army->GetOwningPlayer()))
		{
			return army;
		}
	}
	return nullptr;
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
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] TPSGameMode::BroadcastMessage([%s]-[%s])"),
		*message.Heading, *message.SubHeading);

	State()->UpdateBroadcastMessage(message);

	CombatLog->LogMessage(FTPSLogMessage(message.Heading, message.SubHeading));
}


void ATPSGameMode::UpdateMatchPhase(ETPSMatchPhase phase)
{
	//SetMatchState()

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
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Spawning Character[%s]..."), *characterId.ToString());

	if (UTPSCharacterInstance* character = State()->GetCharacter(characterId))
	{
		if (auto team = character->GetAssignedTeam()) {
			character->SpawnActor(DefaultCharacterTemplate, FindSpawnPointForCharacter(character));
			UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Character[%s] spawned."), *characterId.ToString());
			return true;
		}
	}
	UE_LOG(LogGameMode, Warning, TEXT("FAILED to spawn Character[%s]!"), *characterId.ToString());
	return false;
}

void ATPSGameMode::SpawnTeams()
{
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::InitializeTeams()"));

	TArray<TObjectPtr<UTPSCharacterInstance>> roster;
	for (auto team : State()->Teams)
	{
		SpawnTeam(team->TeamID);
	}
}

bool ATPSGameMode::SpawnTeam(ETPSTeamID teamId)
{
	UE_LOG(LogGameMode, Verbose, TEXT("[SERVER]-[GameMode] TPSGameMode::SpawnTeam([%s])"), *TPSTeamIdToString(teamId));

	bool success = true;
	if (UTPSTeamInstance* t = State()->GetTeam(teamId))
	{
		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Spawning Team[%s]..."), *TPSTeamIdToString(t->TeamID));

		for (auto army : t->GetArmies()) {
			success &= SpawnArmy(army->ArmyID);
		}

		// TODO: Spawn non-Army member units

		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Team[%s] spawned."), *TPSTeamIdToString(t->TeamID));
	}
	return success;
}

bool ATPSGameMode::SpawnArmy(FTPSArmyID armyId)
{
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Spawning Army[%s]..."), *armyId.ToString());
	if (UTPSArmyInstance* army = State()->GetArmy(armyId))
	{
		if (auto unit = army->GetRootUnit()) {
			SpawnUnit(army->GetRootUnit()->UnitID);
			UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Army[%s] spawned."), *armyId.ToString());
			return true;
		}
	}
	UE_LOG(LogGameMode, Warning, TEXT("[SERVER]-[GameMode] FAILED to spawn Army[%s]!"), *armyId.ToString());
	return false;
}

bool ATPSGameMode::SpawnUnit(FTPSUnitID unitId)
{
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Spawning Unit[%s]..."), *unitId.ToString());
	if (UTPSCommandUnit* unit = State()->GetUnit(unitId))
	{
		if (auto team = unit->GetAssignedTeam()) {
			_SpawnUnit(unit, team->SpawnPool);
			UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Unit[%s] spawned."), *unitId.ToString());
			return true;
		}
	}
	UE_LOG(LogGameMode, Warning, TEXT("[SERVER]-[GameMode] FAILED to spawn Unit[%s]!"), *unitId.ToString());
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
	if (!IsValid(player) || !IsValid(army)) { return; }

	player->AssignToArmy(army);
	army->BindToPlayer(player);
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Player[%s] assigned to Army[%s]."), *player->ID.ToString(), *army->ArmyID.ToString());
}

void ATPSGameMode::AssignPlayerToTeam(ATPSPlayerState* player, UTPSTeamInstance* team) const
{
	if (!IsValid(player) || !IsValid(team)) { return; }

	player->AssignToTeam(team);
	team->AddPlayer(player);
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Player[%s] assigned to Team[%s]."), *player->ID.ToString(), *TPSTeamIdToString(team->TeamID));
}







void ATPSGameMode::InitializeTeams()
{
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Initializing Teams..."));
	for (auto teamConfig : TeamDefinitionMap)
	{
		TeamFactory->CreateTeam(teamConfig.Key);
		TeamFactory->ConfigureTeam(teamConfig.Key, teamConfig.Value->Definition);
	}
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Teams Initialized."));
}


void ATPSGameMode::PopulateTeams()
{
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Populating Teams..."));
	TArray<TObjectPtr<UTPSCharacterInstance>> roster;
	for (auto team : State()->Teams)
	{
		TeamFactory->PopulateTeam(team->TeamID, roster);
	}
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Teams Populated."));
}

void ATPSGameMode::KillCharacter_Implementation(const FTPSCharacterID characterId)
{
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] RECEIVED REQUEST - KillCharacter([%s])."), *characterId.Guid.ToString());

	if (auto character = State()->GetCharacter(characterId))
	{
		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Eliminating Character[%s]..."), *characterId.ToString());
		character->Die();

		auto report = GenerateEliminationReportForCharacterDeath(character);
		CombatLog->LogElimination(report);

		// Broadcast Event -> BP GameMode handler
		OnCharacterElimination(report);
		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Character[%s] Eliminated."), *characterId.ToString());
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
	if (!IsValid(controller) || !IsValid(character)) { return; }
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] RECEIVED REQUEST - PossessCharacterActor([%s])."), *character->GetName());

	if (AllowPlayerHotSwap && character->CanBePossessedByPlayer)
	{
		controller->Possess(character);
		UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Controller[%s] granted possession of Character[%s]."), *controller->GetName(), *character->GetName());
	}
	else
	{
		UE_LOG(LogGameMode, Warning, TEXT("[SERVER]-[GameMode] Possession REJECTED - Character[%s] cannot be possessed."), *character->GetName());
	}
}

void ATPSGameMode::RequestPossessCharacterInstance_Implementation(ATPSPlayerController* controller, UTPSCharacterInstance* character)
{
	if (!IsValid(controller) || !IsValid(character)) { return; }
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] RECEIVED REQUEST - PossessCharacterInstance(Controller[%s], Character[%s])."), *controller->GetName(), *character->CharacterID.ToString());

	if (ATPSPlayerState* player = controller->GetPlayerState<ATPSPlayerState>()) {
		if (CanCharacterBePossessedByPlayer(player, character))
		{
			controller->Possess(character->GetSpawnedActor());
			UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Controller[%s] granted possession of Character[%s]."), *controller->GetName(), *character->GetName());
		}
		else
		{
			UE_LOG(LogGameMode, Warning, TEXT("[SERVER]-[GameMode] Possession REJECTED - Character[%s] cannot be possessed."), *character->CharacterID.ToString());
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

	return character->CanBePossessed()
		&& player->GetAssignedTeamID() == character->GetAssignedTeamID()
		&& player->GetAssignedArmy() == character->GetAssignedArmy();
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
	UE_LOG(LogGameMode, Log, TEXT("[SERVER]-[GameMode] Performing CONFIG Update..."));

	FTPSGameConfiguration configuration = {
		CVarGlobalCharacterDebugMode->GetInt(),
		CVarGlobalFogDensity->GetFloat()
	};
	UE_LOG(LogGameMode, Log, TEXT("|- INPUT CharacterDEBUG: %i\n|- INPUT FogDensity: %f"),
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
