// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GAS/GASAbilitySet.h"

#include "TPSPlayerState.generated.h"


UCLASS()
class TPS_MULTIPLAYER_API ATPSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATPSPlayerState();
	~ATPSPlayerState();

protected:
	virtual void BeginPlay() override;

//~ ============================================================= ~//
//  ABILITY SYSTEM WIRING
//~ ============================================================= ~//
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

private:
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent{ nullptr };

	////////////////////////////////////////////////////////
	// GAS Initialization
protected:
	void SetupInitialAbilitiesAndEffects();

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UAbilitySet* InitialAbilitySet{ nullptr };

	// Gameplay Effect used to initialize attribute values on spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	////////////////////////////////////////////////////////
	// Input Routing
public:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingContext{ nullptr };

	UPROPERTY(EditAnywhere)
	FAbilityInputBindings AbilityInputBindings;
};
