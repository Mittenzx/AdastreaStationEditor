// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "StationDesignerTypes.h"
#include "ConnectionPoint.generated.h"

/**
 * Component representing a connection point on a station module
 * Handles connection validation, snapping, and visualization
 */
UCLASS(ClassGroup=(StationBuilder), meta=(BlueprintSpawnableComponent))
class MODULARSTATIONDESIGNER_API UConnectionPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UConnectionPointComponent();

	// Connection properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Connection")
	EConnectionType ConnectionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Connection")
	EConnectionSize ConnectionSize;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Connection")
	bool bIsOccupied;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Connection")
	AActor* ConnectedModule;
	
	// Connection methods
	UFUNCTION(BlueprintCallable, Category="Connection")
	bool CanConnectTo(UConnectionPointComponent* OtherPoint) const;
	
	UFUNCTION(BlueprintCallable, Category="Connection")
	bool ConnectTo(UConnectionPointComponent* OtherPoint);
	
	UFUNCTION(BlueprintCallable, Category="Connection")
	void Disconnect();

	// Distance threshold for snapping (in cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connection")
	float SnapDistance;

protected:
	virtual void BeginPlay() override;
	
private:
	// Helper method to check type compatibility
	bool AreTypesCompatible(EConnectionType TypeA, EConnectionType TypeB) const;
	
	// Helper method to check size compatibility
	bool AreSizesCompatible(EConnectionSize SizeA, EConnectionSize SizeB) const;
};
