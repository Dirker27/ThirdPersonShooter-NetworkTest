// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
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

public:
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	////////////////////////////////////////////////////////
	// Player-Based Abilities
public:
	// Abilities that will be granted to controlled pawns on possession.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UAbilitySet* PlayerAbilitySet{ nullptr };

	// Control bindings for player-based abilities.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FAbilityInputBindings PlayerAbilityInputBindings;
};
