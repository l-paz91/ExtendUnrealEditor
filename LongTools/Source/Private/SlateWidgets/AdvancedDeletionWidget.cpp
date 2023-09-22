// -----------------------------------------------------------------------------
// copyright longPointer* 2023

// -----------------------------------------------------------------------------

#include "SlateWidgets/AdvancedDeletionWidget.h"

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
				.AutoHeight()
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
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();

	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(STextBlock)
			.Text(FText::FromString(DisplayAssetName))
		];

	return ListViewRowWidget;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
