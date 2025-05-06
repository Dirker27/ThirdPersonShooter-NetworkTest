// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

#include "GASAbilitySet.h"

#include "TPSAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UTPSAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);



UCLASS()
class TPS_MULTIPLAYER_API UTPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

//~ ======================================================================== ~//
//  CONFIGURATION
//~ ======================================================================== ~//
protected:
	// Abilities native to the player
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UAbilitySet* BaseAbilitySet{ nullptr };

	// Maps UE::InputActions to TPS::EAbilityInput for triggering GAS abilities.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UAbilityInputBindings* InputBindings;

	// Gameplay Effects used to initialize attribute values on spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

public:
	void InitializeBaseAbilitiesAndEffects();


//~ ======================================================================== ~//
//  GAS Documentation Suggestions (/u/tranek)
//~ ======================================================================== ~//
public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this AbilitySystem receives damage.
	virtual void ReceiveDamage(UTPSAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	virtual void AbilityLocalInputPressed(int32 InputID) override;






//~ ======================================================================== ~//
//  ABILITIES MANAGEMENT
//~ ======================================================================== ~//

	////////////////////////////////////////////////////////
	// Abilities CRUD
public:
	void GrantBaseAbilities(UAbilitySet* abilitySet);
	void GrantPlayerBasedAbilities(UAbilitySet* abilitySet);
	void RevokePlayerBasedAbilities();

protected:
	// Handles of Abilities native to the pawn
	//   (EquipItem, Crouch/Boost, etc)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<FGameplayAbilitySpecHandle> BaseAbilitySpecHandles {};

	// Handles of Abilities granted by a possessing player
	//   (HotSwap, DisplayMenu, etc)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<FGameplayAbilitySpecHandle> PlayerBasedAbilitySpecHandles {};

private:
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(UAbilitySet* abilitySet);

	UFUNCTION(BlueprintCallable)
	void RevokeAbilities(TArray<FGameplayAbilitySpecHandle> abilitySpecHandles);




//~ ======================================================================== ~//
//  INPUT ROUTING
//~ ======================================================================== ~//
public:
	void BindToInputComponent(UEnhancedInputComponent* inputComponent);
	void ReleaseAbilityBindingsFromInputComponent(UEnhancedInputComponent* inputComponent);

private:
	void AbilityInputBindingPressedHandler(EAbilityInput abilityInput);
	void AbilityInputBindingReleasedHandler(EAbilityInput abilityInput);
};
