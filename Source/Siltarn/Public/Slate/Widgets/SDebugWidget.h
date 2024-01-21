#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "SlateBasics.h"

class SILTARN_API SDebugWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SDebugWidget) {} 

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }

	void Construct(const FArguments& p_InArgs);

private:

	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
