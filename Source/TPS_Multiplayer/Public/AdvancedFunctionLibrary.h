// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AdvancedFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TPS_MULTIPLAYER_API UAdvancedFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Actor", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass"))
	static class AActor* GetNearestActorOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FVector Location, float Radius = 512.f);
};
