// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Team/TPSTeam.h"

#include "TPSWorldManager.generated.h"


// To be owned by the GameMode
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSWorldManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSWorldManager();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<UTPSTeam>> ActiveTeams;

    
};
