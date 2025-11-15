// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSAIBehavior.generated.h"

/**
 * Data state accessed by a character's AI that is accessible from outside
 *   actors.
 */
UCLASS(BlueprintType)
class UTPSAIBehavior : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Target;
};
