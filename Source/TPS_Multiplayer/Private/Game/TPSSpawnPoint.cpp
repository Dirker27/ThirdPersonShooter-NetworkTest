// (C) ToasterCat Studios 2025

#include "World/TPSSpawnPoint.h"

#include "Character/TPSCharacter.h"

static int _spawn_id = 0;
ATPSSpawnPoint::ATPSSpawnPoint(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	//Collider->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	//ID = _spawn_id++;
	//Collider->ShapeColor = TPSTeamIdToColor(TeamID);
}

void ATPSSpawnPoint::SetColliderColor(FColor color)
{
	Collider->ShapeColor = color;
}

int ATPSSpawnPoint::GenerateUniqueSpawnId()
{
	return _spawn_id++;
}


bool ATPSSpawnPoint::IsSpawnAvailable()
{
	TArray<AActor*> actors;
	Collider->GetOverlappingActors(actors, ATPSCharacter::StaticClass());
	
	return actors.IsEmpty();
}
