// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSUnitHierarchy.h"
#include "TPSUnitID.h"
#include "TPSUnitLevel.h"

#include "Character/TPSCharacterInstance.h"

#include "TPSHierarchicalCollection.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnitUpdate);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitHierarchy Hierarchy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSUnitLevel> UnitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber;

	// Broadcast Delegate - Team/Character/Player Roster Updated
	UPROPERTY(BlueprintAssignable)
	FUnitUpdate UnitUpdate;

protected:
	// TODO: Make Generic/Template
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UTPSCharacterInstance>> Members;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTPSHierarchicalCollection> ParentCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UTPSHierarchicalCollection>> SubCollections;

//~ ======================================================================== ~//
//  PUBLIC OPERATIONS
//~ ======================================================================== ~//
public:
	UFUNCTION(BlueprintCallable)
	bool AddMember(UTPSCharacterInstance* member);
	UFUNCTION(BlueprintCallable)
	bool AddSubCollection(UTPSHierarchicalCollection* subCollection);

	UFUNCTION(BlueprintCallable)
	bool RemoveMember(const int idNumber);
	UFUNCTION(BlueprintCallable)
	bool RemoveSubCollection(const int idNumber);

	UFUNCTION(BlueprintCallable)
	bool CanAddMember(const UTPSCharacterInstance* member) const;
	UFUNCTION(BlueprintCallable)
	bool CanAddSubCollection(const UTPSHierarchicalCollection* subCollection) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UTPSHierarchicalCollection*> GetAllSubCollections();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetMember(int targetIdNumber);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UTPSCharacterInstance*> GetAllMembers();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ETPSUnitLevel LevelDown(ETPSUnitLevel level);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ETPSUnitLevel LevelUp(ETPSUnitLevel level);

};

