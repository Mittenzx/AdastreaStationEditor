// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationCommandManager.h"

void FStationCommandManager::ExecuteCommand(TSharedPtr<IStationCommand> Command, FStationDesign& Design)
{
	if (!Command.IsValid())
	{
		return;
	}
	
	// Execute the command
	Command->Execute(Design);
	
	// Add to undo stack
	UndoStack.Add(Command);
	
	// Clear redo stack (new action invalidates redo)
	RedoStack.Empty();
	
	// Limit history size
	if (UndoStack.Num() > MaxHistorySize)
	{
		UndoStack.RemoveAt(0);
	}
	
	UE_LOG(LogTemp, Verbose, TEXT("Command executed: %s"), *Command->GetDescription());
}

bool FStationCommandManager::Undo(FStationDesign& Design)
{
	if (UndoStack.Num() == 0)
	{
		return false;
	}
	
	// Pop command from undo stack
	TSharedPtr<IStationCommand> Command = UndoStack.Pop();
	
	// Undo the command
	Command->Undo(Design);
	
	// Add to redo stack
	RedoStack.Add(Command);
	
	UE_LOG(LogTemp, Log, TEXT("Command undone: %s"), *Command->GetDescription());
	return true;
}

bool FStationCommandManager::Redo(FStationDesign& Design)
{
	if (RedoStack.Num() == 0)
	{
		return false;
	}
	
	// Pop command from redo stack
	TSharedPtr<IStationCommand> Command = RedoStack.Pop();
	
	// Re-execute the command
	Command->Execute(Design);
	
	// Add back to undo stack
	UndoStack.Add(Command);
	
	UE_LOG(LogTemp, Log, TEXT("Command redone: %s"), *Command->GetDescription());
	return true;
}

FString FStationCommandManager::GetUndoDescription() const
{
	if (UndoStack.Num() > 0)
	{
		return UndoStack.Last()->GetDescription();
	}
	return TEXT("Nothing to undo");
}

FString FStationCommandManager::GetRedoDescription() const
{
	if (RedoStack.Num() > 0)
	{
		return RedoStack.Last()->GetDescription();
	}
	return TEXT("Nothing to redo");
}

void FStationCommandManager::ClearHistory()
{
	UndoStack.Empty();
	RedoStack.Empty();
	UE_LOG(LogTemp, Log, TEXT("Command history cleared"));
}
