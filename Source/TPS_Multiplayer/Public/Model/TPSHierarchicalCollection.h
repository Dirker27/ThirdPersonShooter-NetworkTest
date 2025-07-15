// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSHierarchicalCollectionLevel.h"

#include "Character/TPSCharacter.h"
#include "Character/TPSCharacterInstance.h"
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

protected:
	// TODO: Make Generic/Template
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UTPSCharacterInstance>> Members;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TObjectPtr<UTPSHierarchicalCollection> ParentCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UTPSHierarchicalCollection>> ChildCollections;

//~ ======================================================================== ~//
//  PUBLIC OPERATIONS
//~ ======================================================================== ~//
public:
	UFUNCTION(BlueprintCallable)
	bool AddMember(UTPSCharacterInstance* member);
	UFUNCTION(BlueprintCallable)
	bool AddSubCollection(UTPSHierarchicalCollection* subCollection);

	UFUNCTION(BlueprintCallable)
	bool RemoveMember(UTPSCharacterInstance* member);
	UFUNCTION(BlueprintCallable)
	bool RemoveSubCollection(UTPSHierarchicalCollection* subCollection);

	UFUNCTION(BlueprintCallable)
	bool CanAddMember(UTPSCharacterInstance* member) const;
	UFUNCTION(BlueprintCallable)
	bool CanAddSubCollection(UTPSHierarchicalCollection* subCollection) const;
};

