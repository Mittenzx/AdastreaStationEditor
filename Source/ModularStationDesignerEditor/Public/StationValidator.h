// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

/**
 * Validation message severity levels
 */
enum class EValidationSeverity : uint8
{
	Info,
	Warning,
	Error
};

/**
 * Structure representing a validation message
 */
struct FValidationMessage
{
	EValidationSeverity Severity;
	FString Message;
	FString ModuleID;  // Optional: specific module with issue

	FValidationMessage()
		: Severity(EValidationSeverity::Info)
		, Message(TEXT(""))
		, ModuleID(TEXT(""))
	{
	}

	FValidationMessage(EValidationSeverity InSeverity, const FString& InMessage, const FString& InModuleID = TEXT(""))
		: Severity(InSeverity)
		, Message(InMessage)
		, ModuleID(InModuleID)
	{
	}
};

/**
 * Station design validation logic
 */
class FStationValidator
{
public:
	// Validate entire station design
	static TArray<FValidationMessage> ValidateStation(const FStationDesign& Design);
	
private:
	// Individual validation checks
	static void CheckRequiredModules(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages);
	static void CheckConnectivity(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages);
	static void CheckPowerBalance(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages);
	static void CheckModuleCompatibility(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages);
};
