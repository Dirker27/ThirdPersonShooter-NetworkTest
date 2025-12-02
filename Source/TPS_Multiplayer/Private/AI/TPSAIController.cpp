// (C) ToasterCat Studios 2025

#include "AI/TPSAIController.h"

ATPSAIController::ATPSAIController(const FObjectInitializer& ObjectInitializer)
{
}


void ATPSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ATPSAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATPSAIController::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
}






//~ ======================================================================== ~//
//  CONTROLLER POSSESS / UN-POSSESS
//~ ======================================================================== ~//

void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ATPSAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ATPSAIController::Reset()
{
	Super::Reset();
}

bool ATPSAIController::LineOfSightTo(const AActor* Other, FVector ViewPoint, bool bAlternateChecks) const
{
	return Super::LineOfSightTo(Other, ViewPoint, bAlternateChecks);
}


//~ ======================================================================== ~//
//  AI CONTROLLER BEHAVIOR
//~ ======================================================================== ~//

void ATPSAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void ATPSAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::Reset();
}

void ATPSAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);
}

void ATPSAIController::SetFocalPoint(FVector NewFocus, EAIFocusPriority::Type InPriority)
{
	Super::SetFocalPoint(NewFocus, InPriority);
}

void ATPSAIController::SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority)
{
	Super::SetFocus(NewFocus, InPriority);
}

void ATPSAIController::ClearFocus(EAIFocusPriority::Type InPriority)
{
	Super::ClearFocus(InPriority);
}

