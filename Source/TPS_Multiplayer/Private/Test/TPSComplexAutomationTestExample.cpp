// (C) ToasterCat Studios 2024

#include "CoreMinimal.h"
#include "Engine.h"
#include "UnrealEngine.h"
#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

#include "Character/TPSCharacter.h"


#if WITH_DEV_AUTOMATION_TESTS


//~ ============================================================= ~//
//  EXPERIMENTAL WORLD LOADER
//   :: https://forums.unrealengine.com/t/how-to-load-a-map-from-an-automation-test/469285
//
//  (Needs work)
//~ ============================================================= ~//

struct FState
{
	UWorld* World;
};

UWorld* GetTestWorld()
{
	const TIndirectArray<FWorldContext>& Contexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : Contexts)
	{
		if (Context.WorldType == EWorldType::Game && Context.World() != nullptr)
		{
			return Context.World();
		}
	}
	return nullptr;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(
	FSetWorldPointer, FState&, State);
bool FSetWorldPointer::Update()
{
	State.World = GetTestWorld();
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(
	FWaitForTicking, FState&, State);
bool FWaitForTicking::Update()
{
	UE_LOG(LogTemp, Warning,
		TEXT("Time is %f."), State.World->GetTimeSeconds());
	return State.World->GetTimeSeconds() > 10.0f;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTickingTest, "BROKEN-AutomationTick.Ticking",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
	bool FTickingTest::RunTest(const FString&)
{
	static FState State;
	ADD_LATENT_AUTOMATION_COMMAND(
		FLoadGameMapCommand(TEXT("TestLevel")))
		ADD_LATENT_AUTOMATION_COMMAND(FSetWorldPointer(State))
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForTicking(State))
		return true;
}

//~ ============================================================= ~//
//  Super Simple Stubs
//~ ============================================================= ~//


/** Super Simple Test **/
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTPSAutomationTest, "TPSAutomationTest.TestDoAThing", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FTPSAutomationTest::RunTest(FString const& Parameters) {
	UE_LOG(LogTemp, Display, TEXT("b1"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTPSFunctionalTest, "TPSFunctionalTest.TestDoAThing", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FTPSFunctionalTest::RunTest(FString const& Parameters) {
	const auto BoolToTest = false;
	TestTrue("bool to test", BoolToTest);

	const auto FloatToTest = 0.F;
	const auto Expected = 10.F;
	const auto Tolerance = KINDA_SMALL_NUMBER;
	TestEqual("float to test", FloatToTest, Expected, Tolerance);

	return true;
}

//~ ============================================================= ~//
//  Example "Complex" Suite
//~ ============================================================= ~//

BEGIN_DEFINE_SPEC(FTPSComplexAutomationTestExample, "TPSComplexAutomationTestExample", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
int UsefulInteger = 0;
void UtilityMethodForTest()
{
	UE_LOG(LogTemp, Display, TEXT("UTILITY INVOKED"));
};
FString MapName = "TestLevel";
UWorld* World;
END_DEFINE_SPEC(FTPSComplexAutomationTestExample)

void FTPSComplexAutomationTestExample::Define() {
	BeforeEach([this]() {
		UE_LOG(LogTemp, Display, TEXT("Pre-Suite BeforeEach()"));

		World = UWorld::CreateWorld(EWorldType::Game, false);
		FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
		WorldContext.SetCurrentWorld(World);

		UGameplayStatics::OpenLevel(World, *MapName); // optional
		UE_LOG(LogTemp, Display, TEXT("Test World initialized."));
	});

	Describe("TestScenario", [this]() {
		BeforeEach([this]() {
			UE_LOG(LogTemp, Display, TEXT("Test Scenario BeforeEach()"));
		});

		It("InvokesUtilities", [this]() {
			UE_LOG(LogTemp, Display, TEXT("UtilityInvoke Test Case"));
			UtilityMethodForTest();
		});

		Describe("NestedTests", [this]() {
			BeforeEach([this]() {
				UE_LOG(LogTemp, Display, TEXT("Pre-Nested BeforeEach()"));
			});

			It("NestedCase01", [this]() {
				UE_LOG(LogTemp, Display, TEXT("Nested Test Case 01"));
			});

			It("NestedCase02", [this]() {
				UE_LOG(LogTemp, Display, TEXT("Nested Test Case 02"));
			});

			AfterEach([this]() {
				UE_LOG(LogTemp, Display, TEXT("Post-Nested AfterEach()"));
			});
		});

		AfterEach([this]() {
			UE_LOG(LogTemp, Display, TEXT("Post-Scenario AfterEach 01"));
		});

		AfterEach([this]() {
			UE_LOG(LogTemp, Display, TEXT("Post-Scenario AfterEach 02"));
			});
		});

	AfterEach([this]() {
		UE_LOG(LogTemp, Display, TEXT("Post-Suite AfterEach()"));

		// Cleanup
		GEngine->DestroyWorldContext(World);
		UE_LOG(LogTemp, Display, TEXT("Test World destroyed."));
	});
}
#endif


//~ ============================================================= ~//
//  Map Configuration Test
//~ ============================================================= ~//

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(TPSWorldTest, "TPSWorld", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
FString MapName = "TestLevel";
UWorld* World;
END_DEFINE_SPEC(TPSWorldTest)

void TPSWorldTest::Define() {
	BeforeEach([this]() {
		World = UWorld::CreateWorld(EWorldType::Game, false);
		FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
		WorldContext.SetCurrentWorld(World);

		UGameplayStatics::OpenLevel(World, *MapName);
		UE_LOG(LogTemp, Display, TEXT("Test World initialized."));
	});

	Describe("Initializes World", [this]() {
		It("Map has correct default configuration", [this]() {

			TestEqual("num players", GEngine->GetNumGamePlayers(GetTestWorld()), 0);
			TestEqual("num actors", GEngine->GetCurrentPlayWorld()->GetActorCount(), 15);

			return true;
		});
	});

	AfterEach([this]() {
		// Cleanup
		GEngine->DestroyWorldContext(World);
		UE_LOG(LogTemp, Display, TEXT("Test World destroyed."));
	});
}

#endif


//~ ============================================================= ~//
//  From Google AI
//~ ============================================================= ~//

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTPSCharacterInitializationTest, "TPSCharacter.Initialization", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTPSCharacterInitializationTest::RunTest(const FString& Parameters)
{
	// Create a test world
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	// Spawn the actor to test
	ATPSCharacter* character = World->SpawnActor<ATPSCharacter>();
	TestNotNull("Character should not be null", character);

	// Initial field values (assumed by other tests)
	TestEqual("Initial Health should be 100", character->CurrentHealth, 100, KINDA_SMALL_NUMBER);
	TestEqual("Initial Armor should be 100", character->CurrentArmor, 100, KINDA_SMALL_NUMBER);
	TestEqual("Initial MovementSpeedModifier should be 1.0", character->MovementSpeedModifier, 1, KINDA_SMALL_NUMBER);
	TestEqual("Initial CharacterState should be Casual", character->CurrentBehaviorState, Casual);
	TestEqual("Initial LocomotionState should be Standing", character->CurrentLocomotionState, Standing);
	TestFalse("Initial IsBoosting should be false", character->IsBoosting);
	TestFalse("Initial IsCrouchInputReceived should be false", character->IsCrouchInputReceived);
	TestFalse("Initial IsCrouching should be false", character->IsCrouching());
	TestTrue("Initial IsAlive should be true", character->IsAlive());


	// Cleanup
	GEngine->DestroyWorldContext(World);

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTPSCharacterBehaviorTest, "TPSCharacter.StateTransitionBehavior", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTPSCharacterBehaviorTest::RunTest(const FString& Parameters)
{
	// Create a test world
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	// Spawn the actor to test
	ATPSCharacter* character = World->SpawnActor<ATPSCharacter>();

	// Apply/revert CharacterState
	character->ApplyBehaviorState(Combat);
	TestEqual("Applied CurrentBehaviorState should be Combat",
	character->CurrentBehaviorState, Combat);
	TestEqual("Applied PreviousBehaviorState should be Casual",
	character->PreviousBehaviorState, Casual);
	//
	character->RevertCharacterState();
	TestEqual("Reverted CurrentBehaviorState should be Casual",
	character->CurrentBehaviorState, Casual);
	TestEqual("Reverted PreviousBehaviorState should be Combat",
	character->PreviousBehaviorState, Combat);

	// Apply/revert LocomotionState
	character->ApplyLocomotionState(Crouching);
	TestEqual("Applied CurrentLocomotionState should be Crouching",
	character->CurrentLocomotionState, Crouching);
	TestEqual("Applied PreviousLocomotionState should be Standing",
	character->PreviousLocomotionState, Standing);
	//
	character->RevertLocomotionState();
	TestEqual("Reverted CurrentLocomotionState should be Standing", 
	character->CurrentLocomotionState, Standing);
	TestEqual("Reverted PreviousLocomotionState should be Crouching",
	character->PreviousLocomotionState, Crouching);

	// Evaluate for Boosting input
	character->IsBoosting = true;
	character->ApplyLocomotionState(
		character->EvaluateLocomotionStateForCurrentInput());
	TestEqual("Character that is Boosting should be Sprinting",
	character->CurrentLocomotionState, Sprinting);

	// Evaluate for Crouching input from Sprinting
	character->IsCrouchInputReceived = true;
	character->ApplyLocomotionState(
		character->EvaluateLocomotionStateForCurrentInput());
	TestEqual("Character should be Crouching when Boosting and CrouchInput is received",
	character->CurrentLocomotionState, Crouching);
	TestTrue("Evaluated CharacterMovement should be Crouching",
	character->IsCrouching());

	// Evaluate for return to Sprinting from Crouch
	character->IsCrouchInputReceived = false;
	character->ApplyLocomotionState(
		character->EvaluateLocomotionStateForCurrentInput());
	TestEqual("Character should be Sprinting when Boosting and CrouchInput is released",
	character->CurrentLocomotionState, Sprinting);
	TestFalse("Evaluated CharacterMovement should no longer be Crouching",
	character->IsCrouching());

	// Evaluate for return to Standing from Sprinting
	character->IsBoosting = false;
	character->ApplyLocomotionState(
		character->EvaluateLocomotionStateForCurrentInput());
	TestEqual("Character should be Standing when no longer Boosting",
	character->CurrentLocomotionState, Standing);

	// Cleanup
	GEngine->DestroyWorldContext(World);

	return true;
}

#endif // WITH_AUTOMATION_TESTS