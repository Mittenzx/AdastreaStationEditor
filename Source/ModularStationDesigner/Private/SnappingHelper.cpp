// Copyright Epic Games, Inc. All Rights Reserved.

#include "SnappingHelper.h"
#include "Engine/World.h"
#include "EngineUtils.h"

UConnectionPointComponent* FSnappingHelper::FindNearestConnectionPoint(
	UConnectionPointComponent* SourcePoint,
	const TArray<UConnectionPointComponent*>& PotentialTargets,
	float MaxSnapDistance)
{
	if (!SourcePoint)
	{
		return nullptr;
	}
	
	UConnectionPointComponent* NearestPoint = nullptr;
	float NearestDistanceSq = MaxSnapDistance * MaxSnapDistance;
	FVector SourceLocation = SourcePoint->GetComponentLocation();
	
	for (UConnectionPointComponent* TargetPoint : PotentialTargets)
	{
		if (!TargetPoint || TargetPoint == SourcePoint)
		{
			continue;
		}
		
		// Check if connection is possible
		if (!SourcePoint->CanConnectTo(TargetPoint))
		{
			continue;
		}
		
		// Calculate distance
		FVector TargetLocation = TargetPoint->GetComponentLocation();
		float DistanceSq = FVector::DistSquared(SourceLocation, TargetLocation);
		
		if (DistanceSq < NearestDistanceSq)
		{
			NearestDistanceSq = DistanceSq;
			NearestPoint = TargetPoint;
		}
	}
	
	return NearestPoint;
}

FTransform FSnappingHelper::CalculateSnapTransform(
	UConnectionPointComponent* SourcePoint,
	UConnectionPointComponent* TargetPoint)
{
	if (!SourcePoint || !TargetPoint)
	{
		return FTransform::Identity;
	}
	
	// Get target point location and rotation
	FVector TargetLocation = TargetPoint->GetComponentLocation();
	FRotator TargetRotation = TargetPoint->GetComponentRotation();
	
	// Connection points should face each other, so rotate 180 degrees
	FRotator AlignedRotation = TargetRotation + FRotator(0, 180, 0);
	
	// Calculate offset from source point to its owner's origin
	AActor* SourceOwner = SourcePoint->GetOwner();
	if (SourceOwner)
	{
		FVector SourcePointOffset = SourcePoint->GetComponentLocation() - SourceOwner->GetActorLocation();
		FRotator SourceOwnerRotation = SourceOwner->GetActorRotation();
		
		// Apply inverse transform to get local offset
		FTransform SourceOwnerTransform(SourceOwnerRotation, SourceOwner->GetActorLocation());
		FVector LocalOffset = SourceOwnerTransform.InverseTransformPosition(SourcePoint->GetComponentLocation());
		
		// Calculate final position by subtracting the rotated offset
		FVector FinalLocation = TargetLocation - AlignedRotation.RotateVector(LocalOffset);
		
		return FTransform(AlignedRotation, FinalLocation);
	}
	
	return FTransform(AlignedRotation, TargetLocation);
}

bool FSnappingHelper::ArePointsWithinSnapRange(
	UConnectionPointComponent* PointA,
	UConnectionPointComponent* PointB,
	float SnapDistance)
{
	if (!PointA || !PointB)
	{
		return false;
	}
	
	// Check compatibility first
	if (!PointA->CanConnectTo(PointB))
	{
		return false;
	}
	
	// Check distance
	float DistanceSq = FVector::DistSquared(
		PointA->GetComponentLocation(),
		PointB->GetComponentLocation());
	
	return (DistanceSq <= SnapDistance * SnapDistance);
}

TArray<UConnectionPointComponent*> FSnappingHelper::GetConnectionPoints(AActor* Actor)
{
	TArray<UConnectionPointComponent*> ConnectionPoints;
	
	if (Actor)
	{
		Actor->GetComponents<UConnectionPointComponent>(ConnectionPoints);
	}
	
	return ConnectionPoints;
}

TArray<UConnectionPointComponent*> FSnappingHelper::FindConnectionPointsInRadius(
	const FVector& WorldLocation,
	float SearchRadius,
	UWorld* World,
	AActor* IgnoreActor)
{
	TArray<UConnectionPointComponent*> FoundPoints;
	
	if (!World)
	{
		return FoundPoints;
	}
	
	// Iterate through all actors with connection points
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		
		// Skip ignored actor
		if (Actor == IgnoreActor)
		{
			continue;
		}
		
		// Get connection points from this actor
		TArray<UConnectionPointComponent*> ActorPoints;
		Actor->GetComponents<UConnectionPointComponent>(ActorPoints);
		
		// Check each point's distance
		for (UConnectionPointComponent* Point : ActorPoints)
		{
			if (Point)
			{
				float DistanceSq = FVector::DistSquared(
					Point->GetComponentLocation(),
					WorldLocation);
				
				if (DistanceSq <= SearchRadius * SearchRadius)
				{
					FoundPoints.Add(Point);
				}
			}
		}
	}
	
	return FoundPoints;
}

FRotator FSnappingHelper::CalculateAlignmentRotation(
	UConnectionPointComponent* SourcePoint,
	UConnectionPointComponent* TargetPoint)
{
	if (!SourcePoint || !TargetPoint)
	{
		return FRotator::ZeroRotator;
	}
	
	// Get target forward direction
	FVector TargetForward = TargetPoint->GetForwardVector();
	
	// Connection points should face each other
	FVector DesiredForward = -TargetForward;
	
	// Calculate rotation from source forward to desired forward
	FVector SourceForward = SourcePoint->GetForwardVector();
	FQuat RotationQuat = FQuat::FindBetweenVectors(SourceForward, DesiredForward);
	
	return RotationQuat.Rotator();
}

FVector FSnappingHelper::SnapToGrid(const FVector& Location, float GridSize)
{
	if (GridSize <= 0.0f)
	{
		return Location;
	}
	
	return FVector(
		FMath::RoundToFloat(Location.X / GridSize) * GridSize,
		FMath::RoundToFloat(Location.Y / GridSize) * GridSize,
		FMath::RoundToFloat(Location.Z / GridSize) * GridSize
	);
}

FRotator FSnappingHelper::SnapRotation(const FRotator& Rotation, float AngleIncrement)
{
	if (AngleIncrement <= 0.0f)
	{
		return Rotation;
	}
	
	return FRotator(
		FMath::RoundToFloat(Rotation.Pitch / AngleIncrement) * AngleIncrement,
		FMath::RoundToFloat(Rotation.Yaw / AngleIncrement) * AngleIncrement,
		FMath::RoundToFloat(Rotation.Roll / AngleIncrement) * AngleIncrement
	);
}
