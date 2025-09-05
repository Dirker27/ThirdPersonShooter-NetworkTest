// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"

#include "TPSBroadcastMessage.generated.h"


USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSBroadcastMessage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Heading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SubHeading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DisplayDuration = 3.f;

    FTPSBroadcastMessage() { }

    FTPSBroadcastMessage(FString heading)
    {
        Heading = heading;
    }

    FTPSBroadcastMessage(FString heading, FString subHeading)
    {
        Heading = heading;
        SubHeading = subHeading;
    }
};




UCLASS(BlueprintType)
class UTPSBroadcastMessageObject : public UObject
{
    GENERATED_BODY()

public:
    UTPSBroadcastMessageObject() { }

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
    {
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        DOREPLIFETIME(ThisClass, Message);
    }

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    FTPSBroadcastMessage Message;
};

