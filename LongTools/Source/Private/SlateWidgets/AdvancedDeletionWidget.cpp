// -----------------------------------------------------------------------------
// copyright longPointer* 2023

// -----------------------------------------------------------------------------

#include "SlateWidgets/AdvancedDeletionWidget.h"

// -----------------------------------------------------------------------------

void SAdvancedDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	// receive the value that gets passed into the widget
	// InArgs._TestString;

	// default slot for our custom slate
	ChildSlot
		// slot 1
		[
			SNew(STextBlock)
			.Text(FText::FromString(InArgs._TestString))
		];
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
