// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Character/TPSCharacter.h"
#include "Character/TPSCharacterInstanceFactory.h"
#include "Player/TPSPlayerController.h"
#include "Game/TPSGameConfiguration.h"
#include "Team/TPSTeamInstanceFactory.h"
#include "Types/TPSReport.h"
#include "World/TPSWorldManager.h"

#include "TPSGameMode.generated.h"

/**
 * 
 *
 * No point in replicating - GameMode is SERVER-ONLY.
 */
UCLASS()
class TPS_MULTIPLAYER_API ATPSGameMode : public AGameMode
{
	GENERATED_BODY()	
public:
	ATPSGameMode();
	~ATPSGameMode();

protected:
	virtual void BeginPlay() override;

//~ ==================================================================== ~//
//  COMPONENTS
//~ ==================================================================== ~//

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TPS")
	TObjectPtr<UTPSWorldManager> WorldManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TPS")
	TObjectPtr<UTPSCombatLog> CombatLog;

//~ ==================================================================== ~//
//  CONFIGURATION
//~ ==================================================================== ~//
public:

	// Should we run in DEBUG mode?
	//
	// TODO: Cascade this setting to peer actors that support Debug modes.
	//   - https://toastercatstudios.atlassian.net/browse/PC-71
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TPS|Debug")
	bool IsDebugEnabled = false;


	////////////////////////////////////////////////////////
	// Game Type / Objectives

	// Display name for the GameType - "DeathMatch", "Capture the Flag", etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPS|GameType")
	FString GameTypeName;

	// Descriptor for the GameType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPS|GameType")
	FString GameTypeDescription;

	// Points required for a team to win the round.
	// TODO: Allow for differing win conditions and target values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPS|GameType")
	int PointsToWin;

	// Time limit for the active phase of the match.
	// TODO: Add time limits for each match phase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPS|GameType")
	double TimeLimitSeconds;

	////////////////////////////////////////////////////////
	// Gameplay Functions

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TPS|Config")
	TSubclassOf<ATPSCharacter> DefaultCharacterTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPS|Config")
	TSubclassOf<ATPSCharacter> BotTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TPS|Config")
	TMap<TEnumAsByte<ETPSTeamID>, TObjectPtr<UTPSTeamDefinition>> TeamDefinitionMap;


	////////////////////////////////////////////////////////
	// Factories / Instance Providers
	//
	// TODO: Abstract Factories to "Provider" classes that can deserialize
	//   instances from Back-End player profiles / DB.

	// Team Instance Provider
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Config")
	TObjectPtr<UTPSTeamInstanceFactory> TeamFactory;

	// Army Instance Provider
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Config")
	TObjectPtr<UTPSArmyInstanceFactory> ArmyFactory;

	// Unit Instance Provider
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Config")
	TObjectPtr<UTPSUnitInstanceFactory> UnitFactory;

	// Character Instance Provider
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Config")
	TObjectPtr<UTPSCharacterInstanceFactory> CharacterFactory;



//~ ============================================================= ~//
//  OPERATIONS
//~ ============================================================= ~//
public:

	////////////////////////////////////////////////////////
	// Gameplay Functions

	// Start the match
	virtual void StartMatch() override;
	virtual void EndMatch() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	//virtual bool ReadyToEndMatch() const override;

	UFUNCTION(BlueprintCallable)
	void BroadcastMessage(FTPSBroadcastMessage message);

	UFUNCTION(BlueprintCallable)
	void UpdateMatchPhase(ETPSMatchPhase phase);






	// Instantiates Teams in GameState with configurations defined in TeamDefinitionMap
	UFUNCTION(BlueprintCallable)
	void InitializeTeams();

	// Populates a Team by instantiating and registering armies and
	//   sub-units with generated Character instances.
	// TODO: Fetch instances from Player-supplied Units and Rosters
	UFUNCTION(BlueprintCallable)
	void PopulateTeams();

	// Scans ALL PlayerStart and SpawnPoint Actors and indexes them accordingly in GameState
	//   Should be invoked *after* Teams have been instantiated in order to index them.
	UFUNCTION(BlueprintCallable)
	void IndexSpawnPoints();


	// Spawn all teams with Instances defined entire team.
	UFUNCTION(BlueprintCallable)
	void SpawnTeams();

	// Spawn an entire team.
	UFUNCTION(BlueprintCallable)
	bool SpawnTeam(ETPSTeamID teamId);

	// Spawn a specific army (including ALL sub-units)
	UFUNCTION(BlueprintCallable)
	bool SpawnArmy(FTPSArmyID unitId);

	// Spawn a specific unit (including sub-units)
	UFUNCTION(BlueprintCallable)
	bool SpawnUnit(FTPSUnitID unitId);

	// Spawn a specific character
	UFUNCTION(BlueprintCallable)
	bool SpawnCharacter(FTPSCharacterID characterId);


private:
	void _SpawnUnit(UTPSCommandUnit* unit, UTPSSpawnPool* spawnPool);


	////////////////////////////////////////////////////////
	// Gameplay Functions
public:
	// Character Death - invoked by admin or character on death detection
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void KillCharacter(const FTPSCharacterID characterId);

	// Event Listener
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterElimination(const FTPSEliminationReport elimination);

	// Respawn
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void RequestRespawn(ATPSPlayerController* playerController);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanRespawn(ATPSPlayerController* playerController);
	UFUNCTION(BlueprintCallable)
	void PerformRespawn(ATPSPlayerController* playerController);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* FindSpawnPointForCharacter(UTPSCharacterInstance* instance);


	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = L"") override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;



	// Helper - Provides TPSGameState to self and peers.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSGameState* State() const { return GetGameState<ATPSGameState>(); }



	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTPSEliminationReport GenerateEliminationReportForCharacterDeath(UTPSCharacterInstance* eliminatee) const;


//~ ==================================================================== ~//
//  CONSOLE COMMANDS (Developer-only API)
//~ ==================================================================== ~//

	////////////////////////////////////////////////////////
	// Visual Debugging

	// Toggle Debug widget visibility for all custom TPS Objects and Characters.
	UFUNCTION(Exec, Category = "Debug|Visibility")
	void DebugGlobal();
	UFUNCTION(Exec, Category = "Debug|Visibility")
	void TPS_ToggleDebugForAllCharacters();

	////////////////////////////////////////////////////////
	// Pawn Possession

	// Possess an Empty Character
	UFUNCTION(Server, Reliable, Category = "Possess")
	void RequestPossession(ATPSPlayerController* controller, ATPSCharacter* character);

	////////////////////////////////////////////////////////
	// Spawn Actors and Bots

	// Spawn a new Player Character
	//   (w/o attached PlayerController / PlayerState)
	UFUNCTION(Server, Reliable, Category = "Spawn|Player")
	void SpawnNewPlayerCharacter(AController* controller);
	UFUNCTION(Exec, Category = "Spawn|Player")
	void SpawnPlayer();
	UFUNCTION(Exec, Category = "Spawn|Player")
	void SpawnPlayers(int numPlayersToSpawn);

	// Spawn a new Bot
	//   (w/o attached AIController)
	UFUNCTION(Server, Reliable, Category = "Spawn|Bot")
	void SpawnNewBot(AController* controller);
	UFUNCTION(Exec, Category = "Spawn|Bot")
	void SpawnBot();
	UFUNCTION(Exec, Category = "Spawn|Bot")
	void SpawnBots(int numBotsToSpawn);

//~ ==================================================================== ~//
//  GAME WORLD CONFIGURATION
//~ ==================================================================== ~//

	UFUNCTION(BlueprintCallable)
	void BindConsoleCallbacks();

	UFUNCTION(BlueprintCallable)
	static FTPSGameConfiguration GetGameConfiguration() {
		return *Configuration;
	}
	static void UpdateGameConfiguration(FTPSGameConfiguration* configuration);

private:
	static FTPSGameConfiguration* Configuration;
};
