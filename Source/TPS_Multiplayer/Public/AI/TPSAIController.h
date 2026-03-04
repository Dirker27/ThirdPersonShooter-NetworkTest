// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "TPSAIController.generated.h"

class ATPSPawn;

UCLASS()
class TPS_MULTIPLAYER_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	//~ Begin AActor Interface
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PostRegisterAllComponents() override;
	//~ End AActor Interface


	//~ Begin AController Interface
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
public:
	virtual void Reset() override;
	virtual bool LineOfSightTo(const AActor* Other, FVector ViewPoint = FVector(ForceInit), bool bAlternateChecks = false) const override;
	//~ End AController Interface


public:
	//////////////////////////////////////////////////////
	// Targeting
	//
	// TODO: Move to "Targeting" struct

	// Provided by Controller (AI or Player) - Will drive "look" location if set.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Targeting")
	TWeakObjectPtr<AActor> TargetActor;

	// Target Location - Provided by LOCAL Controller (NOT replicated)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Targeting")
	FVector TargetLookLocation;

	//~ Begin AIController Behavior
public:
	virtual void SetPawn(APawn* InPawn) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;
	virtual void SetFocalPoint(FVector NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay) override;
	virtual void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay) override;
	virtual void ClearFocus(EAIFocusPriority::Type InPriority) override;
	//~ End AIController Behavior
};
