// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/DefaultPawn.h"

#include "GAS/TPSAbilitySystemComponent.h"

#include "TPSPawn.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSPawn : public ADefaultPawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPawn();
	~ATPSPawn();

	// UE Implementables
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif

//~ ======================================================================== ~//
//  ABILITY SYSTEM
//~ ======================================================================== ~//
protected:
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UTPSAbilitySystemComponent* AbilitySystemComponent{ nullptr };

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

};
