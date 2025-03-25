// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Character/TPSCharacterBodyType.h"

#include "TPSCharacterSkinConfiguration.generated.h"


USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSCharacterSkin
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMesh* BaseMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<UMaterial*> MeshMaterials;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<UMaterialInstance*> MeshMaterialInstances;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UClass* AnimationBlueprint;
};


UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterSkinConfiguration : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSCharacterSkin Skin;
};