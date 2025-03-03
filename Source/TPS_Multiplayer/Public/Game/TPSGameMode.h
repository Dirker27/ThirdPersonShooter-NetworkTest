// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSAICharacter.h"
#include "GameFramework/GameMode.h"

#include "Character/TPSCharacter.h"
#include "Character/TPSPlayerCharacter.h"
#include "Player/TPSPlayerController.h"
#include "Game/TPSGameConfiguration.h"
#include "GameFramework/PlayerStart.h"

#include "TPSGameMode.generated.h"


UCLASS()
class TPS_MULTIPLAYER_API ATPSGameMode : public AGameMode
{
	GENERATED_BODY()	
public:
	ATPSGameMode();
	~ATPSGameMode();

//~ ============================================================= ~//
//  ATTRIBUTES
//~ ============================================================= ~//
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game|Debug")
	bool IsDebugEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Configuration")
	TSubclassOf<ATPSPlayerCharacter> PlayerCharacterTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Configuration")
	TSubclassOf<ATPSAICharacter> BotTemplate;


//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//

	////////////////////////////////////////////////////////
	// Gameplay Functions

	// Respawn
	UFUNCTION(BlueprintCallable)
	bool RequestRespawn();
	UFUNCTION(BlueprintCallable)
	void PerformRespawn(ATPSPlayerController* playerController);


	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerStart* FindSpawnPoint();

//~ ==================================================================== ~//
//  CONSOLE COMMANDS (Developer-only API)
//~ ==================================================================== ~//

	////////////////////////////////////////////////////////
	// Visual Debugging

	UFUNCTION(Exec, Category = "Debug|Visibility")
	void TPS_ToggleDebugForAllCharacters();
	UFUNCTION(Exec, Category = "Debug|Visibility")
	void DebugGlobal();


	////////////////////////////////////////////////////////
	// Spawn Actors and Bots

	// Spawn a new Player Character
	//   (w/o PlayerController / PlayerState)
	UFUNCTION(Exec, Category = "Spawn|Player")
	void TPS_SpawnNewPlayerCharacter();
	UFUNCTION(Exec, Category = "Spawn|Player")
	void SpawnPlayer();
	UFUNCTION(Exec, Category = "Spawn|Player")
	void SpawnPlayers(int numPlayersToSpawn);

	// Spawn a new Bot
	UFUNCTION(Exec, Category = "Spawn|Bot")
	void TPS_SpawnNewBot();
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
