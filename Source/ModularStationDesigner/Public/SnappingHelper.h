// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionPoint.h"

/**
 * Helper utilities for module snapping and connection
 * Provides algorithms for finding nearby connection points and calculating snap transforms
 */
class FSnappingHelper
{
public:
	/**
	 * Find the nearest connection point within snap distance
	 * @param SourcePoint The connection point to snap from
	 * @param PotentialTargets Array of potential target connection points
	 * @param MaxSnapDistance Maximum distance for snapping (in cm)
	 * @return The nearest compatible connection point, or nullptr if none found
	 */
	static UConnectionPointComponent* FindNearestConnectionPoint(
		UConnectionPointComponent* SourcePoint,
		const TArray<UConnectionPointComponent*>& PotentialTargets,
		float MaxSnapDistance = 100.0f);
	
	/**
	 * Calculate the transform needed to snap one module to another via connection points
	 * @param SourcePoint Connection point on the module being moved
	 * @param TargetPoint Connection point to snap to
	 * @return Transform that positions the source module correctly
	 */
	static FTransform CalculateSnapTransform(
		UConnectionPointComponent* SourcePoint,
		UConnectionPointComponent* TargetPoint);
	
	/**
	 * Check if two connection points are within snapping range
	 * @param PointA First connection point
	 * @param PointB Second connection point
	 * @param SnapDistance Distance threshold for snapping
	 * @return True if points are within snap range and compatible
	 */
	static bool ArePointsWithinSnapRange(
		UConnectionPointComponent* PointA,
		UConnectionPointComponent* PointB,
		float SnapDistance);
	
	/**
	 * Get all connection points from an actor
	 * @param Actor The actor to search
	 * @return Array of connection point components
	 */
	static TArray<UConnectionPointComponent*> GetConnectionPoints(AActor* Actor);
	
	/**
	 * Find all connection points within a certain radius
	 * @param WorldLocation Center of search sphere
	 * @param SearchRadius Radius to search within
	 * @param World World context
	 * @param IgnoreActor Actor to exclude from search
	 * @return Array of connection points within radius
	 */
	static TArray<UConnectionPointComponent*> FindConnectionPointsInRadius(
		const FVector& WorldLocation,
		float SearchRadius,
		UWorld* World,
		AActor* IgnoreActor = nullptr);
	
	/**
	 * Calculate alignment rotation between two connection points
	 * Connection points should face each other when connected
	 * @param SourcePoint Connection point on source module
	 * @param TargetPoint Connection point on target module
	 * @return Rotation adjustment needed
	 */
	static FRotator CalculateAlignmentRotation(
		UConnectionPointComponent* SourcePoint,
		UConnectionPointComponent* TargetPoint);
	
	/**
	 * Grid snapping for module placement
	 * @param Location Location to snap
	 * @param GridSize Size of grid cells (in cm)
	 * @return Snapped location
	 */
	static FVector SnapToGrid(const FVector& Location, float GridSize = 100.0f);
	
	/**
	 * Angle snapping for rotation
	 * @param Rotation Rotation to snap
	 * @param AngleIncrement Angle increment in degrees (e.g., 15, 30, 45, 90)
	 * @return Snapped rotation
	 */
	static FRotator SnapRotation(const FRotator& Rotation, float AngleIncrement = 90.0f);
};
