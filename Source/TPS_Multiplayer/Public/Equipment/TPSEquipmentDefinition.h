// Copyright Epic Games, Inc. All Rights Reserved.

////////////////////////////////////////////////////////////////
///
/// IMPORTED FROM LYRA
///
////////////////////////////////////////////////////////////////

#pragma once

#include "TPSEquipmentInstance.h"
#include "Containers/Array.h"
#include "GAS/GASAbilitySet.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "TPSEquipmentDefinition.generated.h"

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
class UTPSEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UTPSEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UTPSEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<const UAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FTPSEquipmentActorToSpawn> ActorsToSpawn;
};
