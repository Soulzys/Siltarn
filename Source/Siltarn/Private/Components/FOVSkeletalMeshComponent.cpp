#include "Components/FOVSkeletalMeshComponent.h"

FMatrix UFOVSkeletalMeshComponent::GetRenderMatrix() const
{
	FMatrix _InverseOldViewProjectionMatrix ;
	FMatrix _NewViewProjectionMatrix        ;

	const FMatrix _ModelMatrix = GetComponentTransform().ToMatrixWithScale();

	APlayerController* _PC = GetWorld()->GetFirstPlayerController();

	if (_PC)
	{
		ULocalPlayer* _LP = Cast<ULocalPlayer>(_PC->Player);

		if (_LP && _LP->ViewportClient && _LP->ViewportClient->Viewport)
		{
			FSceneViewFamilyContext _ViewFamilyContext
			(
				FSceneViewFamily::ConstructionValues(_LP->ViewportClient->Viewport, GetWorld()->Scene, _LP->ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true)
			);

			FVector  _ViewLocation ;
			FRotator _ViewRotation ;
			FSceneView* _SceneView = _LP->CalcSceneView(&_ViewFamilyContext, _ViewLocation, _ViewRotation, _LP->ViewportClient->Viewport);

			if (!_SceneView)
			{
				return _ModelMatrix;
			}

			const float _DesiredFOV        = 60.0f                     ; // This is the FOV for this Component
			const float _DesiredHalfFOVRad = _DesiredFOV * PI / 360.0f ;

			const auto  _ViewportSize              = _LP->ViewportClient->Viewport->GetSizeXY() ;
			const float _Width                     = _ViewportSize.X                            ;
			const float _Height                    = _ViewportSize.Y                            ;
			const float _TargetWidth               = 1920.0f                                    ;
			const float _TargetHeight              = 1080.0f                                    ;
			const float _NearClippingPlaneDistance = _SceneView->NearClippingDistance           ;

			const FMatrix _NewProjectionMatrix     = FReversedZPerspectiveMatrix(_DesiredHalfFOVRad, _TargetWidth, _TargetHeight, _NearClippingPlaneDistance) ;
			const FMatrix _ViewMatrix              = _SceneView->ViewMatrices.GetViewMatrix()                                                     ;
			const FMatrix _InverseViewMatrix       = _SceneView->ViewMatrices.GetInvViewMatrix()                                                  ;
			const FMatrix _ProjectionMatrix        = _SceneView->ViewMatrices.GetProjectionMatrix()                                               ;
			const FMatrix _InverseProjectionMatrix = _SceneView->ViewMatrices.GetInvProjectionMatrix()                                            ;

			_NewViewProjectionMatrix        = _ViewMatrix              * _NewProjectionMatrix ;
			_InverseOldViewProjectionMatrix = _InverseProjectionMatrix * _InverseViewMatrix   ;
		}
	}

	return _ModelMatrix * _NewViewProjectionMatrix * _InverseOldViewProjectionMatrix;
}