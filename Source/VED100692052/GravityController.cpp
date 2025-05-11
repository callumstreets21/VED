// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGravityController::BeginPlay()
{
	SmoothedControlRotation = GetControlRotation();
}

void AGravityController::UpdateRotation(float DeltaTime)
{
	FVector GravityDirection = FVector::DownVector;
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}

	FRotator ViewRotation = GetControlRotation();

	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, GravityDirection);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);
		ViewRotation = WarpedCameraRotation.Rotator();
	}
	LastFrameGravity = GravityDirection;

	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);

	FRotator DeltaRot(RotationInput);

	if (PlayerCameraManager)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn());

		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);

		// Zero the roll for horizontal camera
		ViewRotation.Roll = 0;

		// Convert back to world space
		FRotator TargetControlRotation = GetGravityWorldRotation(ViewRotation, GravityDirection);

		// --- Interpolation step ---
		const float InterpSpeed = 5.0f; // Tweak as needed for smoothing
		SmoothedControlRotation = FMath::RInterpTo(SmoothedControlRotation, TargetControlRotation, DeltaTime, InterpSpeed);

		SetControlRotation(SmoothedControlRotation);
	}

	APawn* const P = GetPawnOrSpectator();
	if (P)
	{
		P->FaceRotation(SmoothedControlRotation, DeltaTime);
	}
}


FRotator AGravityController::GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

FRotator AGravityController::GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}