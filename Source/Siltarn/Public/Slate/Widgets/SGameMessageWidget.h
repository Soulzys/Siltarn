#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SGameMessageWidget, Log, All);

class AGameplayHUD;

class SILTARN_API SGameMessageWidget : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SGameMessageWidget) {} 

	SLATE_ARGUMENT(TWeakObjectPtr<AGameplayHUD>, a_HUDOwner)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }

	void Construct(const FArguments& p_InArgs);

	void SET_TextMessage(const FText& p_TextMessage);

private:

	TWeakObjectPtr<AGameplayHUD> m_HUDOwner = nullptr;
	
	TSharedPtr<STextBlock> m_TextBlock = nullptr;

	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
