// (C) ToasterCat Studios 2025

#pragma once

#include "Game/TPSGameState.h"

#include "TPSCharacterInstanceFactory.generated.h"

/**
 * GameMode worker sub-component to manage Team compositions and schema.
 *
 * TODO: Rename to "TeamFactory"/"TeamWorker"?
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterInstanceFactory : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSCharacterInstanceFactory();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UTPSCharacterConfiguration> DefaultCharacterTemplate;

public:
    static UTPSCharacterInstance* NewCharacterInstance();
};