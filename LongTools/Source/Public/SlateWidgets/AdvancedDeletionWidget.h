// -----------------------------------------------------------------------------
// copyright longPointer* 2023.

#pragma once
// -----------------------------------------------------------------------------

#include <Widgets/SCompoundWidget.h>

// -----------------------------------------------------------------------------

class SAdvancedDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvancedDeletionTab) {}

	SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, AssetDataArray)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay,
		const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData> AssetDataToDisplay);

	TArray<TSharedPtr<FAssetData>> AssetDataUnderSelectedFolder;

	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
