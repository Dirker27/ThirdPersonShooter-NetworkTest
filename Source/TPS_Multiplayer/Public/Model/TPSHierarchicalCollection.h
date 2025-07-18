// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSHierarchicalCollectionLevel.h"

#include "Character/TPSCharacterInstance.h"
#include "GameFramework/Actor.h"

#include "TPSHierarchicalCollection.generated.h"

/**
 * A Hierarchical Trie-structure that represents a group of entities (team characters)
 *
 * Can have nested sub-collections that have lower rank than the given node. 
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSHierarchicalCollection : public UObject
{
	GENERATED_BODY()

public:
	UTPSHierarchicalCollection();


	// Composite UnitID - unique to the active trie (but not globally)
	//
	// Level + UnitID == "Squad 2"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitID UnitID;

protected:
	// TODO: Make Generic/Template
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<uint8, TObjectPtr<UTPSCharacterInstance>> Members;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTPSHierarchicalCollection> ParentCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<uint8, TObjectPtr<UTPSHierarchicalCollection>> ChildCollections;

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



	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSHierarchicalCollection* GetChildCollection(FTPSUnitID id);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UTPSHierarchicalCollection*> GetAllChildCollections();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetMember(uint8 targetIdNumber);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UTPSCharacterInstance*> GetAllMembers();


protected:
	void UpdateUnitIDForMember(UTPSCharacterInstance* member, uint8 id);
	void UpdateUnitIDForSubCollection(UTPSHierarchicalCollection* subCollection, uint8 id);

private:
	uint8 GetNextAvailableIDNumberForMember() const;
	uint8 GetNextAvailableIDNumberForSubCollection() const;



public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ETPSHierarchicalLevel LevelDown(ETPSHierarchicalLevel level);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ETPSHierarchicalLevel LevelUp(ETPSHierarchicalLevel level);

};

