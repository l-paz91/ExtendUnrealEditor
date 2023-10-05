// -----------------------------------------------------------------------------
// copyright longPointer* 2023

// -----------------------------------------------------------------------------

#include "SlateWidgets/AdvancedDeletionWidget.h"

#include "DebugUtilities.h"

#include <Widgets/Layout/SScrollBox.h>

// -----------------------------------------------------------------------------

void SAdvancedDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	// receive the value that gets passed into the widget
	// InArgs._TestString;

	AssetDataUnderSelectedFolder = InArgs._AssetDataArray;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;

	// default slot for our custom slate
	ChildSlot
		// slot 1
		[
			// main vertical box
			SNew(SVerticalBox)

			// first vertical slot for title text
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Advanced Deletion")))
				.Font(TitleTextFont)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::White)
			]

			// second vertical slot for drop down to specify listing condition
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			]

			// third slot for the asset list
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)	// add vertical scrolling
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SListView<TSharedPtr<FAssetData>>)
					.ItemHeight(24)
					.ListItemsSource(&AssetDataUnderSelectedFolder)
					.OnGenerateRow(this, &SAdvancedDeletionTab::OnGenerateRowForList)
				]
			]

			// fourth slot for 3 buttons
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			]
		];
}

// -----------------------------------------------------------------------------

TSharedRef<ITableRow> SAdvancedDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!AssetDataToDisplay->IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();

	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(SHorizontalBox)

			// first slot for check box
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(.05f)
			[
				ConstructCheckBox(AssetDataToDisplay)
			]

			// second slot for displaying asset class name

			// third slot for displaying asset name
			+SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(DisplayAssetName))
			]

			// fourth slot for a button
			
		];

	return ListViewRowWidget;
}

// -----------------------------------------------------------------------------

TSharedRef<SCheckBox> SAdvancedDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData> AssetDataToDisplay)
{
	TSharedRef<SCheckBox> Checkbox = 
		SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvancedDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);

	return Checkbox;
}

// -----------------------------------------------------------------------------

void SAdvancedDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		DebugUtilities::PrintToEditorWindow(AssetData->AssetName.ToString() + TEXT(" is unchecked"), FColor::Red);
		break;
	case ECheckBoxState::Checked:
		DebugUtilities::PrintToEditorWindow(AssetData->AssetName.ToString() + TEXT(" is checked"), FColor::Green);
		break;
	case ECheckBoxState::Undetermined:
		break;
	default:
		break;
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
