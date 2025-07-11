// (C) ToasterCat Studios 2025

#include "Game/TPSSpawnPoint.h"

#include "Character/TPSCharacter.h"

static int _spawn_id = 0;
ATPSSPawnPoint::ATPSSPawnPoint(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnCollider"));

	ID = _spawn_id++;
	//Collider->ShapeColor = TPSTeamIdToColor(TeamID);
}

void ATPSSPawnPoint::SetColliderColor(FColor color)
{
	//Collider->ShapeColor = color;
}

int ATPSSPawnPoint::GenerateUniqueSpawnId()
{
	return _spawn_id++;
}


bool ATPSSPawnPoint::IsSpawnAvailable()
{
	TArray<AActor*> actors;
	Collider->GetOverlappingActors(actors, ATPSCharacter::StaticClass());

	return actors.IsEmpty();
}
