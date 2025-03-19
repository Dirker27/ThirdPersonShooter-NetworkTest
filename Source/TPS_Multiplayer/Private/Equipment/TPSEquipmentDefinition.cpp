// Copyright Epic Games, Inc. All Rights Reserved.


////////////////////////////////////////////////////////////////
///
/// IMPORTED FROM LYRA
///
////////////////////////////////////////////////////////////////


#include "Equipment/TPSEquipmentDefinition.h"
#include "Equipment/TPSEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSEquipmentDefinition)

UTPSEquipmentDefinition::UTPSEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UTPSEquipmentInstance::StaticClass();
}

