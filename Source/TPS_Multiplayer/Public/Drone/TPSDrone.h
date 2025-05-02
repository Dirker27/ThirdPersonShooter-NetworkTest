// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Pawn/TPSPawn.h"

#include "TPSDrone.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSDrone : public ATPSPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSDrone();
	~ATPSDrone();

	// UE Implementables
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
