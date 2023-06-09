// Copyright Epic Games, Inc. All Rights Reserved.

// -----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// -----------------------------------------------------------------------------

class FLongToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void InitCBMenuExtension();
	void AddCBMenuEntry(FMenuBuilder& InMenuBuilder);
	void OnDeleteUnusedAssetButtonClicked();

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& InSelectedPaths);
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
