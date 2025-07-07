// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSSpawnPoint.h"
#include "GameFramework/GameMode.h"

#include "Character/TPSCharacter.h"
#include "Player/TPSPlayerController.h"
#include "Game/TPSGameConfiguration.h"
#include "GameFramework/PlayerStart.h"
#include "Team/TPSTeamManager.h"
#include "World/TPSCombatLog.h"
#include "World/TPSWorldManager.h"

#include "TPSGameMode.generated.h"


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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSWorldManager> WorldManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSTeamManager> TeamManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSCombatLog> CombatLog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int, TObjectPtr<ATPSSPawnPoint>> SpawnPointsById;

//~ ============================================================= ~//
//  ATTRIBUTES
//~ ============================================================= ~//
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game|Debug")
	bool IsDebugEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Configuration")
	TSubclassOf<ATPSCharacter> PlayerCharacterTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Configuration")
	TSubclassOf<ATPSCharacter> BotTemplate;


//~ ============================================================= ~//
//  OPERATIONS
//~ ============================================================= ~//

	////////////////////////////////////////////////////////
	// Gameplay Functions

	void InitializeTeams();
	void IndexSpawnPointsForTeams();

	void SpawnRedTeam();
	void SpawnBlueTeam();


	////////////////////////////////////////////////////////
	// Gameplay Functions

	// Respawn
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void RequestRespawn(ATPSPlayerController* playerController);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanRespawn(ATPSPlayerController* playerController);
	UFUNCTION(BlueprintCallable)
	void PerformRespawn(ATPSPlayerController* playerController);


	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerStart* FindSpawnPoint();









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
//  CONFIGURATION
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
