// (C) ToasterCat Studios 2024


#include "AdvancedFunctionLibrary.h"

#include <Kismet/GameplayStatics.h>

class AActor* UAdvancedFunctionLibrary::GetNearestActorOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FVector Location, float Radius)
{
    AActor* NearestActor = NULL;

    float NearestDistance = Radius;

    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, OutActors);

    for (AActor* Actor : OutActors) {
        float Distance = FVector::Distance(Location, Actor->GetActorLocation());

        if (Distance < NearestDistance)
        {
            NearestActor = Actor;
            NearestDistance = Distance;
        }
    }

    return NearestActor;
}