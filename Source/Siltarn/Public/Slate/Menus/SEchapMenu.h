#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "SlateBasics.h"

class AGameplayHUD;

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SEchapMenu, Log, All);

class SILTARN_API SEchapMenu : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SEchapMenu) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AGameplayHUD>, a_HUDOwner)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void Construct(const FArguments& p_InArgs);

private:

	FReply QUIT_Game();
	FReply CLOSE_Menu();

private:

	TWeakObjectPtr<AGameplayHUD> m_HUDOwner = nullptr;

	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
