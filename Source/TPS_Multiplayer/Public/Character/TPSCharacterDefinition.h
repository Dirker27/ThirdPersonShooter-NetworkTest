// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSCharacterInstance.h"

#include "TPSCharacterDefinition.generated.h"

/**
 * "Duct-Tape"s a Character Instance to an associated Actor (ACharacter) class.
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UTPSCharacterDefinition : public UObject
{
	GENERATED_BODY()

public:
	UTPSCharacterDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) {}

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UTPSCharacterInstance> InstanceType;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TSubclassOf<AActor>> ActorsToSpawn;
};
