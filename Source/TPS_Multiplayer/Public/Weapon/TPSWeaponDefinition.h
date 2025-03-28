// (C) ToasterCat Studios 2025

#pragma once

#include "Equipment/TPSEquipmentItemDefinition.h"

#include "TPSWeaponDefinition.generated.h"

USTRUCT(BlueprintType)
struct FTPSWeaponActorToSpawn : public FTPSEquipmentActorToSpawn
{
	GENERATED_BODY()

	FTPSWeaponActorToSpawn()
	{}
};



UCLASS()
class UTPSWeaponDefinition : public UTPSEquipmentItemDefinition
{
	GENERATED_BODY()

public:
	UTPSWeaponDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) {};
};
