// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

/**
 * Base class for undoable commands
 * Implements the Command design pattern for undo/redo functionality
 */
class IStationCommand
{
public:
	virtual ~IStationCommand() {}
	
	/** Execute the command */
	virtual void Execute(FStationDesign& Design) = 0;
	
	/** Undo the command */
	virtual void Undo(FStationDesign& Design) = 0;
	
	/** Get a description of this command for UI display */
	virtual FString GetDescription() const = 0;
};

/**
 * Command to add a module to the station
 */
class FAddModuleCommand : public IStationCommand
{
public:
	FAddModuleCommand(const FModulePlacement& InModule)
		: Module(InModule)
	{
	}
	
	virtual void Execute(FStationDesign& Design) override
	{
		Design.Modules.Add(Module);
	}
	
	virtual void Undo(FStationDesign& Design) override
	{
		// Remove the module by ID
		Design.Modules.RemoveAll([this](const FModulePlacement& M)
		{
			return M.ModuleID == Module.ModuleID;
		});
	}
	
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("Add Module: %s"), *Module.ComponentName);
	}
	
private:
	FModulePlacement Module;
};

/**
 * Command to remove a module from the station
 */
class FRemoveModuleCommand : public IStationCommand
{
public:
	FRemoveModuleCommand(const FString& InModuleID)
		: ModuleID(InModuleID)
	{
	}
	
	virtual void Execute(FStationDesign& Design) override
	{
		// Find and store the module before removing it
		for (const FModulePlacement& M : Design.Modules)
		{
			if (M.ModuleID == ModuleID)
			{
				RemovedModule = M;
				break;
			}
		}
		
		// Remove the module
		Design.Modules.RemoveAll([this](const FModulePlacement& M)
		{
			return M.ModuleID == ModuleID;
		});
	}
	
	virtual void Undo(FStationDesign& Design) override
	{
		// Re-add the removed module
		Design.Modules.Add(RemovedModule);
	}
	
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("Remove Module: %s"), *RemovedModule.ComponentName);
	}
	
private:
	FString ModuleID;
	FModulePlacement RemovedModule;
};

/**
 * Command to move a module
 */
class FMoveModuleCommand : public IStationCommand
{
public:
	FMoveModuleCommand(const FString& InModuleID, const FTransform& InNewTransform)
		: ModuleID(InModuleID)
		, NewTransform(InNewTransform)
	{
	}
	
	virtual void Execute(FStationDesign& Design) override
	{
		// Find module and store old transform
		for (FModulePlacement& M : Design.Modules)
		{
			if (M.ModuleID == ModuleID)
			{
				OldTransform = M.Transform;
				M.Transform = NewTransform;
				break;
			}
		}
	}
	
	virtual void Undo(FStationDesign& Design) override
	{
		// Restore old transform
		for (FModulePlacement& M : Design.Modules)
		{
			if (M.ModuleID == ModuleID)
			{
				M.Transform = OldTransform;
				break;
			}
		}
	}
	
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("Move Module: %s"), *ModuleID);
	}
	
private:
	FString ModuleID;
	FTransform NewTransform;
	FTransform OldTransform;
};

/**
 * Command to connect two modules
 */
class FConnectModulesCommand : public IStationCommand
{
public:
	FConnectModulesCommand(const FString& InModuleA, const FString& InModuleB)
		: ModuleAID(InModuleA)
		, ModuleBID(InModuleB)
	{
	}
	
	virtual void Execute(FStationDesign& Design) override
	{
		// Add connections
		for (FModulePlacement& M : Design.Modules)
		{
			if (M.ModuleID == ModuleAID)
			{
				M.ConnectedModuleIDs.AddUnique(ModuleBID);
			}
			else if (M.ModuleID == ModuleBID)
			{
				M.ConnectedModuleIDs.AddUnique(ModuleAID);
			}
		}
	}
	
	virtual void Undo(FStationDesign& Design) override
	{
		// Remove connections
		for (FModulePlacement& M : Design.Modules)
		{
			if (M.ModuleID == ModuleAID)
			{
				M.ConnectedModuleIDs.Remove(ModuleBID);
			}
			else if (M.ModuleID == ModuleBID)
			{
				M.ConnectedModuleIDs.Remove(ModuleAID);
			}
		}
	}
	
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("Connect Modules: %s <-> %s"), *ModuleAID, *ModuleBID);
	}
	
private:
	FString ModuleAID;
	FString ModuleBID;
};

/**
 * Command manager for handling undo/redo operations
 */
class FStationCommandManager
{
public:
	/**
	 * Execute a command and add it to history
	 */
	void ExecuteCommand(TSharedPtr<IStationCommand> Command, FStationDesign& Design);
	
	/**
	 * Undo the last command
	 */
	bool Undo(FStationDesign& Design);
	
	/**
	 * Redo the previously undone command
	 */
	bool Redo(FStationDesign& Design);
	
	/**
	 * Check if undo is available
	 */
	bool CanUndo() const { return UndoStack.Num() > 0; }
	
	/**
	 * Check if redo is available
	 */
	bool CanRedo() const { return RedoStack.Num() > 0; }
	
	/**
	 * Get description of next undo action
	 */
	FString GetUndoDescription() const;
	
	/**
	 * Get description of next redo action
	 */
	FString GetRedoDescription() const;
	
	/**
	 * Clear all command history
	 */
	void ClearHistory();
	
	/**
	 * Get command history size
	 */
	int32 GetHistorySize() const { return UndoStack.Num() + RedoStack.Num(); }
	
	/**
	 * Set maximum history size (default: 50)
	 */
	void SetMaxHistorySize(int32 Size) { MaxHistorySize = FMath::Max(1, Size); }
	
private:
	TArray<TSharedPtr<IStationCommand>> UndoStack;
	TArray<TSharedPtr<IStationCommand>> RedoStack;
	int32 MaxHistorySize = 50;
};
