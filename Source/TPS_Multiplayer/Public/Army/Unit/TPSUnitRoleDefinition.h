// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Army/Unit/TPSUnitRole.h"
#include "Character/Types/TPSCharacterRank.h"
#include "Equipment/TPSEquipmentLoadout.h"

#include "TPSUnitRoleDefinition.generated.h"


USTRUCT(BlueprintType)
struct FTPSUnitRoleDefinitionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSUnitRole> Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSEquipmentLoadout> Loadout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSCharacterRank> Rank;
};

UCLASS()
class UTPSUnitRoleDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FTPSUnitRoleDefinitionData Definition;
};
