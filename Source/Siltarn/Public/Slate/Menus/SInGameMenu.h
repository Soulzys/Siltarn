#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "SlateBasics.h"

class AGameplayHUD   ;
class SItemInGameTag ;

class SILTARN_API SInGameMenu : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SInGameMenu) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AGameplayHUD>, a_HUDOwner)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }

	void Construct               (const FArguments& p_InArgs)                                             ;
	void UPDATE_ItemTagBoxSize   (int32 p_DesiredWidth, int32 p_DesiredHeight)                            ;

	// New inventory way
	void DISPLAY_ItemTag(const FString& p_ItemName);

private:

	TWeakObjectPtr<AGameplayHUD> m_HUDOwner = nullptr;

	TSharedPtr<SItemInGameTag> m_ItemTag       = nullptr;
	TSharedPtr<SBox          > m_ItemTagBox    = nullptr;
	TSharedPtr<SBorder       > m_PaddingBorder = nullptr;

	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
