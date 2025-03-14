#include "Player/TPSPlayerCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Player/TPSPlayerController.h"


/**
 * Stolen from:
 *   https://forums.unrealengine.com/t/player-camera-in-controller-instead-of-character/64165/2
 */
void ATPSPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	/*
	if ((PendingViewTarget.Target != NULL) && BlendParams.bLockOutgoing && OutVT.Equal(ViewTarget))
	{
		return;
	}

	FMinimalViewInfo OrigPOV = OutVT.POV;
	OutVT.POV.FOV = DefaultFOV;
	OutVT.POV.OrthoWidth = DefaultOrthoWidth;
	OutVT.POV.bConstrainAspectRatio = false;
	OutVT.POV.ProjectionMode = this->bIsOrthographic ? ECameraProjectionMode::Orthographic : ECameraProjectionMode::Perspective;
	OutVT.POV.PostProcessBlendWeight = 1.0f;
	bool bDoNotApplyModifiers = false;

	ATPSPlayerController* BZGame_Controller = Cast<ATPSPlayerController>(GetOwningPlayerController());
	if (BZGame_Controller != NULL)
	{
		UCameraComponent* ViewCam = BZGame_Controller->GetViewCamera();
		OutVT.POV.Location = ViewCam->GetComponentLocation();
		OutVT.POV.Rotation = ViewCam->GetComponentRotation();
		OutVT.POV.FOV = ViewCam->FieldOfView;
		OutVT.POV.AspectRatio = ViewCam->AspectRatio;
		OutVT.POV.bConstrainAspectRatio = ViewCam->bConstrainAspectRatio;
		OutVT.POV.ProjectionMode = ViewCam->ProjectionMode;
		OutVT.POV.OrthoWidth = ViewCam->OrthoWidth;
		OutVT.POV.PostProcessBlendWeight = ViewCam->PostProcessBlendWeight;

		if (BZGame_Controller->GetViewCamera()->PostProcessBlendWeight > 0.0f)
		{
			OutVT.POV.PostProcessSettings = ViewCam->PostProcessSettings;
		}

		if (!bDoNotApplyModifiers || this->bAlwaysApplyModifiers)
		{
			ApplyCameraModifiers(DeltaTime, OutVT.POV);
		}

		SetActorLocationAndRotation(OutVT.POV.Location, OutVT.POV.Rotation, false);
		UpdateCameraLensEffects(OutVT);
	}
	else
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
	}
	*/
	Super::UpdateViewTarget(OutVT, DeltaTime);
}
