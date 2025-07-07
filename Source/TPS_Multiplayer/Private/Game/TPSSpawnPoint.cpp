// (C) ToasterCat Studios 2025

#include "Game/TPSSpawnPoint.h"

ATPSSPawnPoint::ATPSSPawnPoint()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
}

