// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSHierarchicalCollectionLevel.h"
#include "Character/TPSCharacter.h"
#include "GameFramework/Actor.h"

#include "TPSHierarchicalCollection.generated.h"

// A Hiererchical Trie-structure
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSHierarchicalCollection : public UObject
{
	GENERATED_BODY()

public:
	UTPSHierarchicalCollection();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ETPSHierarchicalLevel> Level;

	// TODO: Make Generic/Template
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ATPSCharacter>> Members;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTPSHierarchicalCollection> ParentCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UTPSHierarchicalCollection>> ChildCollections;

//~ ======================================================================== ~//
//  PUBLIC OPERATIONS
//~ ======================================================================== ~//
public:
	bool Add(ATPSCharacter* member);
	bool Add(UTPSHierarchicalCollection* subCollection);
	bool Remove(ATPSCharacter* member);
	bool Remove(UTPSHierarchicalCollection* subCollection);

	bool CanAddToCollection(ATPSCharacter* member) const;
	bool CanAddToCollection(UTPSHierarchicalCollection* subCollection) const;
};

