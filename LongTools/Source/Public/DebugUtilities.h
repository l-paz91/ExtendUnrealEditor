// -----------------------------------------------------------------------------
// copyright longPointer* 2023

#pragma once

// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <Runtime\Core\Public\Misc\MessageDialog.h>
#include <Runtime\Slate\Public\Widgets\Notifications\SNotificationList.h>
#include <Runtime\Slate\Public\Framework\Notifications\NotificationManager.h>

// -----------------------------------------------------------------------------

namespace DebugUtilities
{
	void PrintToEditorWindow(const FString& pMessage, const FColor& pColor)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.f, pColor, pMessage);
		}
	}

	// -------------------------------------------------------------------------

	void PrintToLog(const FString& pMessage)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *pMessage);
	}

	// -------------------------------------------------------------------------

	EAppReturnType::Type ShowMsgBox(EAppMsgType::Type pMsgType, const FString& pMsg, const FString& pTitle)
	{
		FText MsgTitle = FText::FromString(pTitle);
		FText MsgBody = FText::FromString(pMsg);

		return FMessageDialog::Open(pMsgType, MsgBody, &MsgTitle);
	}

	// -------------------------------------------------------------------------

	void ShowNotifyInfo(const FString& pMsg)
	{
		FNotificationInfo NotifyInfo(FText::FromString(pMsg));
		NotifyInfo.bUseLargeFont = true;
		NotifyInfo.FadeOutDuration = 7.f;

		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
