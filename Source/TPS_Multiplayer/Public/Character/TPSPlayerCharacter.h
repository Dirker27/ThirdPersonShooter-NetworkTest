////////////////////////////////////////////////////////////////
/// DEPRECATED!!!
///
/// Moved to consolidated TPSCharacter
///
////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GAS/GASAbilitySet.h"
#include "Character/TPSCharacter.h"

#include "TPSPlayerCharacter.generated.h"

UCLASS()
class ATPSPlayerCharacter : public ATPSCharacter
{
    GENERATED_BODY()
	
//~ ============================================================= ~//
//  ABILITY SYSTEM WIRING
//~ ============================================================= ~//
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

	// Bind Input->ASC
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Bind to ASC in PlayerState
	void PossessedBy(AController* NewController) override;
	void OnRep_PlayerState() override;

protected:
	void InitPlayer();
	void AbilityInputBindingPressedHandler(EAbilityInput abilityInput);
	void AbilityInputBindingReleasedHandler(EAbilityInput abilityInput);

private:
	UPROPERTY(EditAnywhere, Category="Input|Binding")
	UInputMappingContext* InputMappingCtx{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Input|Binding")
	FAbilityInputBindings AbilityInputBndgs;
};

