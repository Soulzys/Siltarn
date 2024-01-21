#include "Slate/Menus/SInGameMenu.h"
#include "Siltarn/Public/Slate/Widgets/SItemInGameTag.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInGameMenu::Construct(const FArguments& p_InArgs)
{
	m_HUDOwner = p_InArgs._a_HUDOwner;

	FSlateFontInfo _FontInfo(m_GeneralStyle.m_Monorita_Font);
	_FontInfo.Size = 20;

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		// Item tag border
		//
		SNew(SBorder)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
		.Padding(0)
		[
			SAssignNew(m_ItemTagBox, SBox)
			.WidthOverride(500.0f)
			.HeightOverride(500.0f)
			[
				SAssignNew(m_PaddingBorder, SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(FMargin(250.0f, 250.0f, 0.0f, 0.0f))
				[
					SAssignNew(m_ItemTag, SItemInGameTag)
					.a_InGameMenu(this)
				]
			]
		]		
	];
}

void SInGameMenu::UPDATE_ItemTagBoxSize(int32 p_DesiredWidth, int32 p_DesiredHeight)
{
	m_ItemTagBox->SetWidthOverride(p_DesiredWidth * 2);
	m_ItemTagBox->SetHeightOverride(p_DesiredHeight * 2);
	m_PaddingBorder->SetPadding(FMargin(p_DesiredWidth, p_DesiredHeight, 0.0f, 0.0f));
}

void SInGameMenu::DISPLAY_ItemTag(const FString& p_ItemName)
{
	m_ItemTag->UPDATE_ItemTag(p_ItemName);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION