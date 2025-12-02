// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "TPSAIController.generated.h"

class ATPSPawn;

UCLASS()
class TPS_MULTIPLAYER_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	virtual void SetPawn(APawn* InPawn) override;

	virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

	virtual FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

	virtual void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const;

	bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

	UE_DEPRECATED_FORGAME(4.13, "This function is now deprecated, please use FindPathForMoveRequest() for adjusting Query or BuildPathfindingQuery() for getting one.")
		virtual bool PreparePathfinding(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query);

	UE_DEPRECATED_FORGAME(4.13, "This function is now deprecated, please use FindPathForMoveRequest() for adjusting pathfinding or path postprocess.")
		virtual FAIRequestID RequestPathAndMove(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query);

	bool PauseMove(FAIRequestID RequestToPause);

	bool ResumeMove(FAIRequestID RequestToResume);

	virtual void StopMovement() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UE_DEPRECATED_FORGAME(4.13, "This function is now deprecated, please use version with EPathFollowingResultDetails parameter.")
		virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	FORCEINLINE FAIRequestID GetCurrentMoveRequestID() const { return GetPathFollowingComponent() ? GetPathFollowingComponent()->GetCurrentRequestId() : FAIRequestID::InvalidRequest; }

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "MoveCompleted"))
	FAIMoveCompletedSignature ReceiveMoveCompleted;

	TSubclassOf<UNavigationQueryFilter> GetDefaultNavigationFilterClass() const { return DefaultNavigationFilterClass; }

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	EPathFollowingStatus::Type GetMoveStatus() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	bool HasPartialPath() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	FVector GetImmediateMoveDestination() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	void SetMoveBlockDetection(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual bool RunBehaviorTree(UBehaviorTree* BTAsset);

protected:
	virtual void CleanupBrainComponent();

public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool UseBlackboard(UBlackboardData* BlackboardAsset, UBlackboardComponent*& BlackboardComponent);

	virtual bool ShouldSyncBlackboardWith(const UBlackboardComponent& OtherBlackboardComponent) const;

	UFUNCTION(BlueprintCallable, Category = "AI|Tasks")
	void ClaimTaskResource(TSubclassOf<UGameplayTaskResource> ResourceClass);

	UFUNCTION(BlueprintCallable, Category = "AI|Tasks")
	void UnclaimTaskResource(TSubclassOf<UGameplayTaskResource> ResourceClass);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUsingBlackBoard(UBlackboardComponent* BlackboardComp, UBlackboardData* BlackboardAsset);

	virtual bool InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset);

public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	FVector GetFocalPoint() const;

	FVector GetFocalPointForPriority(EAIFocusPriority::Type InPriority) const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const;

	UFUNCTION(BlueprintCallable, Category = "AI", meta = (DisplayName = "SetFocalPoint", ScriptName = "SetFocalPoint", Keywords = "focus"))
	void K2_SetFocalPoint(FVector FP);

	UFUNCTION(BlueprintCallable, Category = "AI", meta = (DisplayName = "SetFocus", ScriptName = "SetFocus"))
	void K2_SetFocus(AActor* NewFocus);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetFocusActor() const;

	FORCEINLINE AActor* GetFocusActorForPriority(EAIFocusPriority::Type InPriority) const { return FocusInformation.Priorities.IsValidIndex(InPriority) ? FocusInformation.Priorities[InPriority].Actor.Get() : nullptr; }

	UFUNCTION(BlueprintCallable, Category = "AI", meta = (DisplayName = "ClearFocus", ScriptName = "ClearFocus"))
	void K2_ClearFocus();


	bool SuggestTossVelocity(FVector& OutTossVelocity, FVector Start, FVector End, float TossSpeed, bool bPreferHighArc, float CollisionRadius = 0, bool bOnlyTraceUp = false);

	//~ Begin AActor Interface
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PostRegisterAllComponents() override;
	//~ End AActor Interface

	//~ Begin AController Interface
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual bool ShouldPostponePathUpdates() const override;
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

#if ENABLE_VISUAL_LOG
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif

	virtual void Reset() override;


	virtual bool LineOfSightTo(const AActor* Other, FVector ViewPoint = FVector(ForceInit), bool bAlternateChecks = false) const override;
	//~ End AController Interface

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true);

	virtual void SetFocalPoint(FVector NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);

	virtual void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);

	virtual void ClearFocus(EAIFocusPriority::Type InPriority);

	void SetPerceptionComponent(UAIPerceptionComponent& InPerceptionComponent);
	//----------------------------------------------------------------------//
	// IAIPerceptionListenerInterface
	//----------------------------------------------------------------------//
	virtual UAIPerceptionComponent* GetPerceptionComponent() override { return GetAIPerceptionComponent(); }

	//----------------------------------------------------------------------//
	// INavAgentInterface
	//----------------------------------------------------------------------//
	virtual bool IsFollowingAPath() const override;
	virtual IPathFollowingAgentInterface* GetPathFollowingAgent() const override { return PathFollowingComponent; }

	//----------------------------------------------------------------------//
	// IGenericTeamAgentInterface
	//----------------------------------------------------------------------//
private:
	FGenericTeamId TeamID;
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

	//----------------------------------------------------------------------//
	// IGameplayTaskOwnerInterface
	//----------------------------------------------------------------------//
	virtual UGameplayTasksComponent* GetGameplayTasksComponent(const UGameplayTask& Task) const override { return GetGameplayTasksComponent(); }
	virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override { return const_cast<ATPSAIController*>(this); }
	virtual AActor* GetGameplayTaskAvatar(const UGameplayTask* Task) const override { return GetPawn(); }
	virtual uint8 GetGameplayTaskDefaultPriority() const { return FGameplayTasks::DefaultPriority - 1; }

	FORCEINLINE UGameplayTasksComponent* GetGameplayTasksComponent() const { return CachedGameplayTasksComponent; }

	// add empty overrides to fix linker errors if project implements a child class without adding GameplayTasks module dependency
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}

	UFUNCTION()
	virtual void OnGameplayTaskResourcesClaimed(FGameplayResourceSet NewlyClaimed, FGameplayResourceSet FreshlyReleased);

	//----------------------------------------------------------------------//
	// Actions
	//----------------------------------------------------------------------//
	bool PerformAction(UPawnAction& Action, EAIRequestPriority::Type Priority, UObject* const Instigator = NULL);

	//----------------------------------------------------------------------//
	// debug/dev-time 
	//----------------------------------------------------------------------//
	virtual FString GetDebugIcon() const;

	// Cheat/debugging functions
	static void ToggleAIIgnorePlayers() { bAIIgnorePlayers = !bAIIgnorePlayers; }
	static bool AreAIIgnoringPlayers() { return bAIIgnorePlayers; }

	static bool bAIIgnorePlayers;

public:
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	UPathFollowingComponent* GetPathFollowingComponent() const { return PathFollowingComponent; }
	UPawnActionsComponent* GetActionsComp() const { return ActionsComp; }
	UFUNCTION(BlueprintPure, Category = "AI|Perception")
	UAIPerceptionComponent* GetAIPerceptionComponent() { return PerceptionComponent; }

	const UAIPerceptionComponent* GetAIPerceptionComponent() const { return PerceptionComponent; }

	UBrainComponent* GetBrainComponent() const { return BrainComponent; }
	const UBlackboardComponent* GetBlackboardComponent() const { return Blackboard; }
	UBlackboardComponent* GetBlackboardComponent() { return Blackboard; }

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	void SetPathFollowingComponent(UPathFollowingComponent* NewPFComponent);*/
};
