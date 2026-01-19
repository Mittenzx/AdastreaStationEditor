// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConnectionPoint.h"

UConnectionPointComponent::UConnectionPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// Default values
	ConnectionType = EConnectionType::Standard;
	ConnectionSize = EConnectionSize::Medium;
	bIsOccupied = false;
	ConnectedModule = nullptr;
	SnapDistance = 50.0f; // 50cm snap threshold
}

void UConnectionPointComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UConnectionPointComponent::CanConnectTo(UConnectionPointComponent* OtherPoint) const
{
	if (!OtherPoint || OtherPoint == this)
	{
		return false;
	}

	// Check if either point is already occupied
	if (bIsOccupied || OtherPoint->bIsOccupied)
	{
		return false;
	}

	// Check type compatibility
	if (!AreTypesCompatible(ConnectionType, OtherPoint->ConnectionType))
	{
		return false;
	}

	// Check size compatibility
	if (!AreSizesCompatible(ConnectionSize, OtherPoint->ConnectionSize))
	{
		return false;
	}

	return true;
}

bool UConnectionPointComponent::ConnectTo(UConnectionPointComponent* OtherPoint)
{
	if (!CanConnectTo(OtherPoint))
	{
		return false;
	}

	// Establish connection
	bIsOccupied = true;
	ConnectedModule = OtherPoint->GetOwner();
	
	OtherPoint->bIsOccupied = true;
	OtherPoint->ConnectedModule = GetOwner();

	return true;
}

void UConnectionPointComponent::Disconnect()
{
	if (ConnectedModule && bIsOccupied)
	{
		// Find the connected point and disconnect it too
		TArray<UConnectionPointComponent*> ConnectedPoints;
		ConnectedModule->GetComponents<UConnectionPointComponent>(ConnectedPoints);
		
		for (UConnectionPointComponent* Point : ConnectedPoints)
		{
			if (Point && Point->ConnectedModule == GetOwner())
			{
				Point->bIsOccupied = false;
				Point->ConnectedModule = nullptr;
				break;
			}
		}
	}

	bIsOccupied = false;
	ConnectedModule = nullptr;
}

bool UConnectionPointComponent::AreTypesCompatible(EConnectionType TypeA, EConnectionType TypeB) const
{
	// Universal connections work with everything
	if (TypeA == EConnectionType::Universal || TypeB == EConnectionType::Universal)
	{
		return true;
	}

	// Standard connections work with Standard and Corridor
	if (TypeA == EConnectionType::Standard)
	{
		return (TypeB == EConnectionType::Standard || TypeB == EConnectionType::Corridor);
	}

	// Corridor connections work with everything except Docking
	if (TypeA == EConnectionType::Corridor)
	{
		return (TypeB != EConnectionType::Docking);
	}

	// Docking connections cannot connect to other Docking connections
	if (TypeA == EConnectionType::Docking)
	{
		return (TypeB != EConnectionType::Docking);
	}

	// Power connections only work with Power
	if (TypeA == EConnectionType::Power)
	{
		return (TypeB == EConnectionType::Power);
	}

	// Same types can connect
	return (TypeA == TypeB);
}

bool UConnectionPointComponent::AreSizesCompatible(EConnectionSize SizeA, EConnectionSize SizeB) const
{
	// Universal size works with everything
	if (SizeA == EConnectionSize::Universal || SizeB == EConnectionSize::Universal)
	{
		return true;
	}

	// Same sizes are always compatible
	if (SizeA == SizeB)
	{
		return true;
	}

	// Adjacent sizes are compatible (with warning)
	int32 SizeDiff = FMath::Abs(static_cast<int32>(SizeA) - static_cast<int32>(SizeB));
	return (SizeDiff == 1);
}
