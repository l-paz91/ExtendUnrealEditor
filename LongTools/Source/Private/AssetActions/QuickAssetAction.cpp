// copyright longPointer* 2023

// -----------------------------------------------------------------------------

#include "AssetActions/QuickAssetAction.h"

#include "LongTools\Public\DebugUtilities.h"

#include <Editor\Blutility\Public\EditorUtilityLibrary.h>
#include <Editor\UnrealEd\Public\ObjectTools.h>
#include <EditorScriptingUtilities\Public\EditorAssetLibrary.h>
#include <Runtime\Core\Public\Misc\MessageDialog.h>
#include <Runtime\Engine\Classes\Materials\MaterialInstanceConstant.h>

// -----------------------------------------------------------------------------

UQuickAssetAction::UQuickAssetAction()
	: UAssetActionUtility()
	, mPrefixMap{ 
		{UBlueprint::StaticClass(), TEXT("BP_")}, 
		{UMaterialInstanceConstant::StaticClass(), TEXT("MI_")}, 
		{UMaterial::StaticClass(), TEXT("M_")} }
{

}

// -----------------------------------------------------------------------------

void UQuickAssetAction::TestFunction()
{
	using namespace DebugUtilities;

	PrintToEditorWindow(TEXT("Working editor window"), FColor::Cyan);
	PrintToLog(TEXT("Working log"));
}

// -----------------------------------------------------------------------------

void UQuickAssetAction::DuplicateAssets(int32 pNumOfDuplicates)
{
	if (pNumOfDuplicates)
	{
		TArray<FAssetData> SelectedAssetData = UEditorUtilityLibrary::GetSelectedAssetData();
		uint32 Counter = 0;

		for (const FAssetData& SelectedAsset : SelectedAssetData)
		{
			for (int32 i = 0; i < pNumOfDuplicates; ++i)
			{
				const FString SourceAssetPath = SelectedAsset.GetSoftObjectPath().GetLongPackageName();
				const FString NewDuplicatedAssetName = SelectedAsset.AssetName.ToString() + TEXT("_") + FString::FromInt(i+1);
				const FString NewPathName = FPaths::Combine(SelectedAsset.PackagePath.ToString(), NewDuplicatedAssetName);
				
				if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
				{
					UEditorAssetLibrary::SaveAsset(NewPathName, false);
					++Counter;
				}
			}
		}

		if (Counter)
		{
			const FString MsgBody = TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files");
			DebugUtilities::ShowNotifyInfo(MsgBody);
		}
	}
	else
	{
		const FString MsgBody = TEXT("Please enter a valid number");
	}
}

// -----------------------------------------------------------------------------

void UQuickAssetAction::AutoPrefix()
{
	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for (UObject* SelectedAsset : SelectedAssets)
	{
		if (SelectedAsset)
		{
			FString* PrefixFound = mPrefixMap.Find(SelectedAsset->GetClass());
			if (!PrefixFound || PrefixFound->IsEmpty())
			{
				DebugUtilities::ShowNotifyInfo(TEXT("Failed to find prefix for class") + SelectedAsset->GetClass()->GetName());
				continue;
			}

			FString OldName = SelectedAsset->GetName();
			if (OldName.StartsWith(*PrefixFound))
			{
				DebugUtilities::ShowNotifyInfo(OldName + TEXT(" already has prefix added"));
				continue;
			}

			if (SelectedAsset->IsA<UMaterialInstanceConstant>())
			{
				OldName.RemoveFromStart(TEXT("M_"));
				OldName.RemoveFromEnd(TEXT("_Inst"));
			}


			const FString NewNameWithPrefix = *PrefixFound + OldName;

			UEditorUtilityLibrary::RenameAsset(SelectedAsset, NewNameWithPrefix);
			++Counter;
		}
	}

	if (Counter)
	{
		DebugUtilities::ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(Counter) + " assets"));
	}
}

// -----------------------------------------------------------------------------

void UQuickAssetAction::RemoveUnusedAssets()
{
	TArray<FAssetData> SelectedAssetData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetData;

	for (const FAssetData& SelectedAsset : SelectedAssetData)
	{
		TArray<FString> PackageReferencersForAsset = UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAsset.GetSoftObjectPath().GetLongPackageName());

		if (!PackageReferencersForAsset.Num())
		{
			UnusedAssetData.Add(SelectedAsset);
		}
	}

	if (!UnusedAssetData.Num())
	{
		DebugUtilities::ShowMsgBox(EAppMsgType::Ok, TEXT("No unused asset found among selected assets."), TEXT("Asset Action"));
		return;
	}

	int32 DeletedAssets = ObjectTools::DeleteAssets(UnusedAssetData);

	if (DeletedAssets)
	{
		DebugUtilities::ShowNotifyInfo(TEXT("Successfully deleted ") + FString::FromInt(DeletedAssets) + TEXT(" unused assets."));
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
