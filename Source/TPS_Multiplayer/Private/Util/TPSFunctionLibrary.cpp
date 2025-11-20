// (C) ToasterCat Studios 2025


#include "Util/TPSFunctionLibrary.h"

#include <Kismet/GameplayStatics.h>

#include "Army/Unit/TPSUnitHierarchy.h"
#include "Character/TPSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Team/TPSTeamID.h"

AActor* UTPSFunctionLibrary::GetNearestActorOfClass(const UObject* worldContextObject, TSubclassOf<AActor> actorClass, FVector location, float radius)
{
    return GetNearestActorOfClassAndIgnore(worldContextObject, actorClass, location, radius, TArray<AActor*>());
}

AActor* UTPSFunctionLibrary::GetNearestActorOfClassAndIgnore(const UObject* worldContextObject, TSubclassOf<AActor> actorClass, FVector location, float radius, TArray<AActor*> toIgnore)
{
    AActor* nearestActor = NULL;

    float nearestDistance = radius;

    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(worldContextObject, actorClass, OutActors);

    for (AActor* actor : OutActors) {
        float distance = FVector::Distance(location, actor->GetActorLocation());

        if (distance < nearestDistance && !toIgnore.Contains(actor))
        {
            nearestActor = actor;
            nearestDistance = distance;
        }
    }

    return nearestActor;
}

ATPSCharacter* UTPSFunctionLibrary::GetNearestPlayableCharacter(const UObject* worldContextObject, FVector location, float radius, TArray<AActor*> toIgnore)
{
    ATPSCharacter* nearestCharacter = nullptr;

    float nearestDistance = radius;

    TArray<AActor*> outActors;
    UGameplayStatics::GetAllActorsOfClass(worldContextObject, ATPSCharacter::StaticClass(), outActors);

    for (AActor* actor : outActors) {
        ATPSCharacter* character = Cast<ATPSCharacter>(actor);
        float distance = FVector::Distance(location, actor->GetActorLocation());

        if (distance < nearestDistance
            && character->CanBePossessedByPlayer
            && character->IsAlive()
            && !toIgnore.Contains(actor))
        {
            nearestCharacter = character;
            nearestDistance = distance;
        }
    }

    return nearestCharacter;
}

FVector2D UTPSFunctionLibrary::CalculateNoise2D(const float pitchDegrees, const float yawDegrees)
{
    FVector randomDirection = UKismetMathLibrary::RandomUnitVector();
    FVector2D offsetDirection = FVector2D(randomDirection.X, randomDirection.Y).GetSafeNormal(0.00001);

    float deltaPitch = UKismetMathLibrary::RandomFloatInRange(
        randomDirection.X * pitchDegrees * -1,
        randomDirection.X * pitchDegrees);
    float deltaYaw = UKismetMathLibrary::RandomFloatInRange(
        randomDirection.Y * yawDegrees * -1,
        randomDirection.Y * yawDegrees);

    return FVector2D(deltaPitch, deltaYaw);
}

FVector UTPSFunctionLibrary::CalculateImpulseJoules(const FVector velocity, const float mass)
{
    UE::Math::TVector<double> direction;
    float magnitude;
    velocity.ToDirectionAndLength(direction, magnitude);

    magnitude = magnitude / 100; // convert to m/s

    // KE == v^2 * (m/2)
    float joules = (magnitude * magnitude) * mass;

    FVector impulseVector = joules * direction;
    return impulseVector;
}


AActor* UTPSFunctionLibrary::LineTrace(const UObject* WorldContextObject, const FVector startLoc, const FVector direction) {
    AActor* hitActor = NULL;

    FVector endLoc = startLoc + (direction * 1000.f);

    ETraceTypeQuery channel = TraceTypeQuery_MAX;
    TArray<AActor*> actorsToIgnore;
    EDrawDebugTrace::Type debugTrace = EDrawDebugTrace::Type::ForOneFrame;
    FHitResult hitResult;

    bool isHit = UKismetSystemLibrary::LineTraceSingle(WorldContextObject, startLoc, endLoc,
        channel, false, actorsToIgnore, debugTrace,
        hitResult,
        true,
        FLinearColor::Red, FLinearColor::Green, 5.f);

    if (isHit) {
        hitActor = hitResult.GetActor();
    }

    return hitActor;
}


void UTPSFunctionLibrary::DrawDebugTrace(const UObject* WorldContextObject, const FVector startLoc, const FVector direction)
{
    UTPSFunctionLibrary::DrawDebugTrace(WorldContextObject, startLoc, direction,
        FLinearColor::MakeRandomColor(), FLinearColor::MakeRandomColor(), 1.f);
}
void UTPSFunctionLibrary::DrawDebugTrace(const UObject* worldContextObject,
                    const FVector startLoc,
                    const FVector direction,
                    const FLinearColor traceColor,
                    const FLinearColor hitColor,
                    const float duration)
{
    FVector endLoc = startLoc + direction;

    ETraceTypeQuery channel = TraceTypeQuery_MAX;
    TArray<AActor*> actorsToIgnore;
    EDrawDebugTrace::Type debugTrace = EDrawDebugTrace::Type::ForDuration;

    FHitResult hitResult;
    UKismetSystemLibrary::LineTraceSingle(worldContextObject, startLoc, endLoc,
        channel, false, actorsToIgnore, debugTrace,
        hitResult,
        true,
        traceColor, hitColor, duration);
}


FColor UTPSFunctionLibrary::GetColorForTeam(const ETPSTeamID teamId)
{
    return TPSTeamIdToColor(teamId);
}

FString UTPSFunctionLibrary::GetNameForUnitID(const FTPSUnitID unitId)
{
    return TPSUnitIdToString(unitId);
}

FString UTPSFunctionLibrary::GetNameForUnitHierarchy(const FTPSUnitHierarchy hierarchy)
{
    return TPSUnitHierarchyToString(hierarchy);
}

int64 UTPSFunctionLibrary::GetHashForUnitID(const FTPSUnitID unitId)
{
    return 0;
}

int64 UTPSFunctionLibrary::GetHashForUnitHierarchy(const FTPSUnitID unitId)
{
    return 0;
}


FString UTPSFunctionLibrary::GetAbbreviationForRank(const ETPSCharacterRank rank)
{
    return TPSCharacterRankAbbreviationString(rank);
}

FTPSUnitID UTPSFunctionLibrary::MakeChildUnitID(FTPSUnitID parentId, ETPSUnitLevel childLevel, int childNumber)
{
    FTPSUnitID id = FTPSUnitID();

    return id;
}



FString UTPSFunctionLibrary::GetFormalDisplayNameForCharacter(const UTPSCharacterInstance* character)
{
    if (!IsValid(character)) {
        return "#CHARACTER#";
    }

    FString displayName = TPSCharacterRankAbbreviationString(character->Identity.Rank) + ". ";
    displayName += character->Identity.FirstName + " ";
    if (!character->Identity.Callsign.IsEmpty())
    {
        displayName += "'" + character->Identity.Callsign + "' ";
    }
    displayName += character->Identity.LastName;

    return displayName;
}

FString UTPSFunctionLibrary::GetShortDisplayNameForCharacter(const UTPSCharacterInstance* character)
{
    return IsValid(character)
            ? FString::Printf(TEXT("%s. %s"),
                *TPSCharacterRankAbbreviationString(character->Identity.Rank),
                *character->Identity.LastName)
            : "#CHARACTER#";
}