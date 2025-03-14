////////////////////////////////////////////////////////////////
/// DEPRECATED!!!
///
/// Moved to consolidated TPSCharacter
///
////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"

#include "Character/TPSCharacter.h"

#include "GAS/Attributes/StandardAttributeSet.h"
#include "GAS/Attributes/CharacterHealthAttributeSet.h"
#include "GAS/Attributes/WeaponAttributeSet.h"

#include "TPSAICharacter.generated.h"

// TODO: Collapse this back into the normal "Character" class.
//   We shouldn't need to split the AI functionality to its own
//   class unless we get really involved with minion bots.
UCLASS()
class TPS_MULTIPLAYER_API ATPSAICharacter : public ATPSCharacter
{
    GENERATED_BODY()

public:
    ATPSAICharacter();

public:
    virtual void BeginPlay() override;

//~ ============================================================= ~//
//  ABILITY SYSTEM WIRING
//~ ============================================================= ~//

protected:
   	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UAbilitySystemComponent* ASC{ nullptr };

public:

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

    //////////////////////////////////////////////////////
    // GAS Attributes

	UPROPERTY(VisibleAnywhere, Category = "Abilities|Attributes", Transient)
	UStandardAttributeSet* StandardAttrs{ nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Abilities|Attributes", Transient)
    UCharacterHealthAttributeSet* CharacterHealthAttrs{ nullptr };

    UPROPERTY(VisibleAnywhere, Category = "Abilities|Attributes", Transient)
    UWeaponAttributeSet* WeaponAttrs{ nullptr };
};

