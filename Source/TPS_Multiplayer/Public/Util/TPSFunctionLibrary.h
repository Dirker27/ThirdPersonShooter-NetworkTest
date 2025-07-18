// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Character/TPSCharacter.h"

#include "TPSFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actor", meta = (WorldContext = "worldContextObject", DeterminesOutputType = "actorClass"))
	static AActor* GetNearestActorOfClass(const UObject* worldContextObject, TSubclassOf<AActor> actorClass, FVector location, float radius = 512.f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actor", meta = (WorldContext = "worldContextObject", DeterminesOutputType = "actorClass"))
	static AActor* GetNearestActorOfClassAndIgnore(const UObject* worldContextObject, TSubclassOf<AActor> actorClass, FVector location, float radius, TArray<AActor*> toIgnore);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actor", meta = (WorldContext = "worldContextObject", DeterminesOutputType = "actorClass"))
	static ATPSCharacter* GetNearestPlayableCharacter(const UObject* worldContextObject, FVector location, float radius, TArray<AActor*> toIgnore);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static AActor* LineTrace(const UObject* WorldContextObject, const FVector startLoc, const FVector direction);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	static void DrawDebugTrace(const UObject* WorldContextObject, const FVector startLoc, const FVector direction);
	static void DrawDebugTrace(const UObject* WorldContextObject, const FVector startLoc, const FVector direction,
		const FLinearColor traceColor, const FLinearColor hitColor, const float duration);
		
	// Calculates impulse in Joules from default unreal units
	//  Velocity = cm/s
	//  Mass = kg
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	static FVector CalculateImpulseJoules(const FVector velocity, const float mass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	static FVector2D CalculateNoise2D(const float pitchDegrees, const float yawDegrees);



	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FColor GetColorForTeam(ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetNameForUnitID(FTPSUnitID unitID);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int64 GetHashForUnitID(FTPSUnitID unitId);

};
