// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "ModuleDiscovery.h"

/**
 * Drag-drop operation for module placement
 */
class FModuleDragDropOp : public FDecoratedDragDropOp
{
public:
	DRAG_DROP_OPERATOR_TYPE(FModuleDragDropOp, FDecoratedDragDropOp)

	/** The module being dragged */
	TSharedPtr<FModuleInfo> ModuleInfo;

	/** Static construction function */
	static TSharedRef<FModuleDragDropOp> New(TSharedPtr<FModuleInfo> InModuleInfo)
	{
		TSharedRef<FModuleDragDropOp> Operation = MakeShared<FModuleDragDropOp>();
		Operation->ModuleInfo = InModuleInfo;
		
		if (InModuleInfo.IsValid())
		{
			Operation->DefaultHoverText = FText::FromString(InModuleInfo->Name);
		}
		else
		{
			Operation->DefaultHoverText = FText::FromString(TEXT("Invalid Module"));
		}
		
		Operation->CurrentHoverText = Operation->DefaultHoverText;
		Operation->Construct();
		return Operation;
	}
};
