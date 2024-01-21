#include "Slate/Widgets/SDebugWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDebugWidget::Construct(const FArguments& p_InArgs)
{
	ChildSlot
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SBox)
		.WidthOverride(50.0f)
		.HeightOverride(50.0f)
		[
			SNew(SBorder)
			.BorderImage(&m_GeneralStyle.m_DebuggingGreen_SlateBrush)
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION