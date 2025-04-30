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

	friend class TPSPlayerController;

public:
	ATPSPlayerState();
	~ATPSPlayerState();

protected:
	virtual void BeginPlay() override;

//~ ============================================================= ~//
//  ABILITY SYSTEM
//~ ============================================================= ~//
private:
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent{ nullptr };

public:
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	////////////////////////////////////////////////////////
	// GAS Initialization
protected:
	void SetupInitialAbilitiesAndEffects();

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UAbilitySet* InitialAbilitySet{ nullptr };
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	////////////////////////////////////////////////////////
	// Input Routing
public:
	UPROPERTY(EditAnywhere)
	FAbilityInputBindings AbilityInputBindings;

	// Input -> ASC wiring (bound by PlayerController)
	void BindInputToASC(UEnhancedInputComponent* input);
	void AbilityInputBindingPressedHandler(EAbilityInput abilityInput);
	void AbilityInputBindingReleasedHandler(EAbilityInput abilityInput);
};
