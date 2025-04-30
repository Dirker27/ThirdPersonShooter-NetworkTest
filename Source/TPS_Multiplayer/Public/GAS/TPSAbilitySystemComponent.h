// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "TPSAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UTPSAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * Initial impl imported from GASDocumentation (tranek)
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UTPSAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	virtual void AbilityLocalInputPressed(int32 InputID) override;
};
