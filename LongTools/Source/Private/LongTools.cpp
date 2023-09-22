// copyright longPointer* 2023

#include "LongTools.h"
#include "LongTools/Public/DebugUtilities.h"
#include "SlateWidgets/AdvancedDeletionWidget.h"

#include <AssetRegistry/AssetRegistryModule.h>
#include <AssetToolsModule.h>
#include <Editor/ContentBrowser/Public/ContentBrowserModule.h>
#include <EditorScriptingUtilities/Public/EditorAssetLibrary.h>
#include <Editor/UnrealEd/Public/ObjectTools.h>
#include <Framework/Docking/TabManager.h>

// -----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "FLongToolsModule"

// -----------------------------------------------------------------------------

namespace LongToolsPrivate
{
	const FName AdvancedDeletionTab = "AdvancedDeletion";
}

void FLongToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// The exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();
	RegisterAdvancedDeletionTab();
}

// -----------------------------------------------------------------------------

void FLongToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

// -----------------------------------------------------------------------------

void FLongToolsModule::InitCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& CBContextMenuExtenders = ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this, &FLongToolsModule::CustomCBMenuExtender);
	CBContextMenuExtenders.Add(CustomCBMenuDelegate);
}

// -----------------------------------------------------------------------------

// define details for the custom menu entry
void FLongToolsModule::AddCBMenuEntry(FMenuBuilder& InMenuBuilder)
{
	InMenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Unused Assets")),		// title text
		FText::FromString(TEXT("Safely delete all unused assets under folder")),	// tooltip text
		FSlateIcon(),	// custom icon
		FExecuteAction::CreateRaw(this, &FLongToolsModule::OnDeleteUnusedAssetButtonClicked)// the actual function to execute
	);

	InMenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Empty Folders")),		// title text
		FText::FromString(TEXT("Safely delete allempty folders")),	// tooltip text
		FSlateIcon(),	// custom icon
		FExecuteAction::CreateRaw(this, &FLongToolsModule::OnDeleteEmptyFoldersMenuEntryClicked)// the actual function to execute
	);

	InMenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Advanced Deletion")),		// title text
		FText::FromString(TEXT("List assets by specific condiion in a tab for deleting")),	// tooltip text
		FSlateIcon(),	// custom icon
		FExecuteAction::CreateRaw(this, &FLongToolsModule::OnAdvancedDeletionButtonClicked)// the actual function to execute
	);
}

// -----------------------------------------------------------------------------

void FLongToolsModule::OnDeleteUnusedAssetButtonClicked()
{
	const FString MsgBoxTitle = TEXT("Delete Assets");

	if (mFolderPathsSelected.Num() > 1)
	{
		DebugUtilities::ShowMsgBox(EAppMsgType::Ok, TEXT("You can only do this to one folder"), MsgBoxTitle);
		return;
	}

	if (mFolderPathsSelected.Num())
	{
		TArray<FString> AssetPathsNames = UEditorAssetLibrary::ListAssets(mFolderPathsSelected[0]);

		// whether there are assets under the folder
		if (!AssetPathsNames.Num())
		{
			DebugUtilities::ShowMsgBox(EAppMsgType::Ok, TEXT("No Assets found under selected folder"), MsgBoxTitle);
			return;
		}

		EAppReturnType::Type ReturnType = DebugUtilities::ShowMsgBox(EAppMsgType::YesNo, TEXT("A total of ") + FString::FromInt(AssetPathsNames.Num()) + TEXT(" found\nWould you like to proceed?"), MsgBoxTitle);
		
		if (ReturnType == EAppReturnType::No)
		{
			return;
		}

		if (ReturnType == EAppReturnType::Yes)
		{
			FixUpRedirectors();
			TArray<FAssetData> UnusedAssetData;

			for (const FString& AssetPathName : AssetPathsNames)
			{
				if (AssetPathName.Contains(TEXT("Developers")) || AssetPathName.Contains(TEXT("Collections")) )
				{
					// don't touch the root folders
					continue;
				}

				if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName))
				{
					continue;
				}

				TArray<FString> PackageReferencersForAsset = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);

				if (!PackageReferencersForAsset.Num())
				{
					const FAssetData FoundUnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
					UnusedAssetData.Add(FoundUnusedAssetData);
				}
			}

			if (UnusedAssetData.Num())
			{
				ObjectTools::DeleteAssets(UnusedAssetData);
			}
			else
			{
				DebugUtilities::ShowMsgBox(EAppMsgType::Ok, TEXT("No unused assets found under selected folder"), MsgBoxTitle);
			}
		}
	}

	//DebugUtilities::PrintToEditorWindow(TEXT("Currently selected folder: ") + mFolderPathsSelected[0], FColor::Green);
}

// -----------------------------------------------------------------------------

void FLongToolsModule::OnDeleteEmptyFoldersMenuEntryClicked()
{
	FixUpRedirectors();

	// filled with assets and folder paths
	TArray<FString> FolderPathsArray = UEditorAssetLibrary::ListAssets(mFolderPathsSelected[0], true, true);
	uint32 FoldersDeleted = 0;

	FString EmptyFolderPathsNames;
	TArray<FString> EmptyFoldersPathsArray;

	for (const FString& FolderPath : FolderPathsArray)
	{
		if (FolderPath.Contains(TEXT("Developers")) || FolderPath.Contains(TEXT("Collections")))
		{
			// don't touch the root folders
			continue;
		}

		if (UEditorAssetLibrary::DoesDirectoryExist(FolderPath))
		{
			if (!UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath))
			{
				// its an empty folder
				EmptyFolderPathsNames.Append(FolderPath);
				EmptyFolderPathsNames.Append(TEXT("\n"));
				EmptyFoldersPathsArray.Add(FolderPath);
			}
		}
	}

	// did we find empty folders?
	if (EmptyFoldersPathsArray.Num())
	{
		EAppReturnType::Type showMsgBox = DebugUtilities::ShowMsgBox(EAppMsgType::OkCancel, TEXT("Emptry folders found in:\n") + EmptyFolderPathsNames + TEXT("\nWould you like to delete all?"), TEXT("Warning"));
		if (showMsgBox == EAppReturnType::Ok)
		{
			for (const FString& EmptyFolderPath : EmptyFoldersPathsArray)
			{
				bool DirectoryDeleted = UEditorAssetLibrary::DeleteDirectory(EmptyFolderPath);
				if (DirectoryDeleted)
				{
					++FoldersDeleted;
				}
				else
				{
					DebugUtilities::PrintToEditorWindow(TEXT("Failed to delete ") + EmptyFolderPath, FColor::Red);
				}
			}

			if (FoldersDeleted)
			{
				DebugUtilities::ShowNotifyInfo(TEXT("Successfully deleted ") + FString::FromInt(FoldersDeleted) + TEXT(" folders"));
			}
		}
	}
	else
	{
		DebugUtilities::ShowMsgBox(EAppMsgType::Ok, TEXT("No empty folders found under selected folder"), TEXT("Message"));
	}
}

// -----------------------------------------------------------------------------

void FLongToolsModule::OnAdvancedDeletionButtonClicked()
{
	// DebugUtilities::PrintToEditorWindow(TEXT("Working"), FColor::Green);

	FGlobalTabmanager::Get()->TryInvokeTab(LongToolsPrivate::AdvancedDeletionTab);
}

// -----------------------------------------------------------------------------

void FLongToolsModule::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFix;

	FModuleManager& ModuleManager = FModuleManager::Get();
	FAssetRegistryModule& AssetRegistryModule = ModuleManager.LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// filter out assets we want under a folder
	FARFilter Filter;
	Filter.bRecursivePaths = true;					// will allow going in sub folders
	Filter.PackagePaths.Emplace("/Game");			// folder path we want filter to go through
	Filter.ClassPaths.Emplace("ObjectRedirector");

	TArray<FAssetData> OutRedirectors;
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors);

	for (const FAssetData& RedirectorData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset()))
		{
			RedirectorsToFix.Add(RedirectorToFix);
		}
	}

	FAssetToolsModule& AssetToolsModule = ModuleManager.LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	AssetToolsModule.Get().FixupReferencers(RedirectorsToFix);
}

// -----------------------------------------------------------------------------

void FLongToolsModule::RegisterAdvancedDeletionTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(LongToolsPrivate::AdvancedDeletionTab,
		FOnSpawnTab::CreateRaw(this, &FLongToolsModule::OnSpawnAdvancedDeletionTab))
		.SetDisplayName(FText::FromString(TEXT("Advanced Deletion")));
}

// -----------------------------------------------------------------------------

TSharedRef<SDockTab> FLongToolsModule::OnSpawnAdvancedDeletionTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		// add a new widget slot
		[
			SNew(SAdvancedDeletionTab)
			.TestString(TEXT("I am passing data"))
		];
}

// -----------------------------------------------------------------------------

// to define the position for inserting menu entry
TSharedRef<FExtender> FLongToolsModule::CustomCBMenuExtender(const TArray<FString>& InSelectedPaths)
{
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender());

	if (InSelectedPaths.Num())
	{
		MenuExtender->AddMenuExtension(
			FName("Delete"),					// extension hook, position to insert
			EExtensionHook::After,				// insert before or after
			TSharedPtr<FUICommandList>(),		// custom hot key
			FMenuExtensionDelegate::CreateRaw(this, &FLongToolsModule::AddCBMenuEntry) // second binding, will define details for this menu entry
		);

		mFolderPathsSelected = InSelectedPaths;
	}

	return MenuExtender;
}

// -----------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLongToolsModule, LongTools)

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
