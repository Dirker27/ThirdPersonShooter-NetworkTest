// (C) ToasterCat Studios 2025

#pragma once

#include "Containers/Array.h"
#include "GAS/GASAbilitySet.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "TPSEquipmentItemDefinition.generated.h"

class AActor;
class ULyraAbilitySet;
class ULyraEquipmentInstance;

USTRUCT()
struct FTPSEquipmentActorToSpawn
{
	GENERATED_BODY()

	FTPSEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};


/**
 * ULyraEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UTPSEquipmentItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UTPSEquipmentItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) {};

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULyraEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<const UAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FTPSEquipmentActorToSpawn> ActorsToSpawn;
};
