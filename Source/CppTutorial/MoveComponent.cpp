// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UMoveComponent::EnableMovement(bool ShouldMove)
{
	MoveEnable = ShouldMove;
	SetComponentTickEnabled(MoveEnable);
}

void UMoveComponent::ResetMovement()
{
	CurDistance = 0.0;
	SetRelativeLocation(StartRelativeLocation);
}

void UMoveComponent::SetMoveDirection(int Direction)
{
	MoveDirection = Direction >= 1 ? 1 : -1;
}

// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// set start Location
	StartRelativeLocation = this->GetRelativeLocation();
	
	MoveOffsetNorm = MoveOffset;
	MoveOffsetNorm.Normalize();
	MaxDistance = MoveOffset.Length();
	SetComponentTickEnabled(MoveEnable);
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MoveEnable)
	{
		CurDistance += DeltaTime * Speed * MoveDirection;
		if (CurDistance >= MaxDistance || CurDistance <= 0)
		{
			MoveDirection *= -1;

			OnEndPointReached.Broadcast(CurDistance >= MaxDistance);

			CurDistance = FMath::Clamp(CurDistance, 0.0f, MaxDistance);
		}
			
	}
	
	// .
	SetRelativeLocation(StartRelativeLocation+MoveOffsetNorm*CurDistance);
}

