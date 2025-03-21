// Copyright Epic Games, Inc. All Rights Reserved.


////////////////////////////////////////////////////////////////
///
/// IMPORTED FROM LYRA
///
////////////////////////////////////////////////////////////////


#include "Equipment/LyraEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraEquipmentDefinition)

ULyraEquipmentDefinition::ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = ULyraEquipmentInstance::StaticClass();
}

