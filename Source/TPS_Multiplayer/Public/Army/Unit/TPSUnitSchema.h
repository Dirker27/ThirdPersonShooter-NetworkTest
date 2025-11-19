// (C) ToasterCat Studios 2025

#pragma once

#include "TPSUnitRoleDefinition.h"
#include "Equipment/TPSEquipmentLoadout.h"

#include "Army/Unit/TPSUnitRole.h"
#include "Army/Unit/TPSUnitLevel.h"

#include "TPSUnitSchema.generated.h"

class UTPSUnitSchema;
/**
 * Schema and rules for the current level of a command unit
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitSchemaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSUnitLevel> Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumSubCollections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumMembers;

	// Explicitly states each unit member's role and loadout.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTPSUnitRoleDefinitionData> MemberDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UTPSUnitSchema>> SubUnitDefinitions;

	// Default loadouts for roles - will be overridden by MemberDefinition if set.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<ETPSUnitRole>, UTPSEquipmentLoadout*> DefaultMemberLoadoutMap;

	// TODO: Leader Inheritance Policy, Structure expand/coalesce rules, additional commands, etc
};


UCLASS(BlueprintType)
class UTPSUnitSchema : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSUnitSchemaData Schema;
};
