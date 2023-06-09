// copyright longPointer 2023*

#pragma once

// -----------------------------------------------------------------------------

#include "CoreMinimal.h"
#include <Editor\Blutility\Classes\AssetActionUtility.h>

#include "QuickAssetAction.generated.h"

// -----------------------------------------------------------------------------

UCLASS()
class LONGTOOLS_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UQuickAssetAction();

	UFUNCTION(CallInEditor)
	void TestFunction();

	UFUNCTION(CallInEditor)
	void DuplicateAssets(int32 pNumOfDuplicates);

	UFUNCTION(CallInEditor)
	void AutoPrefix();

	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets();

private:
	void FixUpRedirectors();

	TMap<UClass*, FString> mPrefixMap;
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
