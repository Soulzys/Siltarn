#pragma once

#include "SlateBasics.h"

#define ITEM_PADDING 10

class SInGameMenu;

class SILTARN_API SItemInGameTag : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SItemInGameTag) {} 

	SLATE_ARGUMENT(SInGameMenu*, a_InGameMenu)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void Construct(const FArguments& p_InArgs);
	
	void UPDATE_ItemTag(const FString& p_ItemName);

private:

	void UPDATE_ItemTagBox();

private:

	TSharedPtr<STextBlock> m_ItemName = nullptr;
	TSharedPtr<SBorder> m_ItemTagBorder = nullptr;

	SInGameMenu* m_InGameMenu = nullptr;

	bool m_bNeedUpdate = false;

	//const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
