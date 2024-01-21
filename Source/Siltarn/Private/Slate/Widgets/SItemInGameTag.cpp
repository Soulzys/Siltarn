#include "Slate/Widgets/SItemInGameTag.h"
#include "Siltarn/Public/Slate/Menus/SInGameMenu.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SItemInGameTag::Construct(const FArguments& p_InArgs)
{
	const FSiltarnGeneralStyleContainerStruct& _GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();

	m_InGameMenu = p_InArgs._a_InGameMenu;

	FSlateFontInfo _ItemNameFont(_GeneralStyle.m_Roboto_Bold_Font);
	_ItemNameFont.Size = 16;

	FSlateFontInfo _MessageFont(_GeneralStyle.m_Roboto_Regular_Font);
	_MessageFont.Size = 13;

	ChildSlot
	.HAlign(HAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SAssignNew(m_ItemTagBorder, SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
		.Padding(FMargin(20.0f, 20.0f, 0.0f, 0.0f)) // This serves as the offset from the cursor at the center of the screen
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 8.0f))
			[
				SAssignNew(m_ItemName, STextBlock)
				.Text(FText::FromString(""))
				.ColorAndOpacity(FLinearColor::White)
				.Font(_ItemNameFont)
			]
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(FMargin(0.0f, 0.0f, 5.0f, 0.0f))
				[
					SNew(SBox)
					.WidthOverride(30.0f)
					.HeightOverride(30.0f)
					[
						SNew(SBorder)
						.BorderImage(&_GeneralStyle.m_Box_Alphabet_SlateBrush)
						.Padding(5)
						[
							SNew(SImage)
							.Image(&_GeneralStyle.m_Alphabet_E_SlateBrush)
						]
					]					
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Get item"))
					.ColorAndOpacity(FLinearColor::White)
					.Font(_MessageFont)
				]				
			]
		]	
	];
}

void SItemInGameTag::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (m_bNeedUpdate == true)
	{
		UPDATE_ItemTagBox();
	}
}

void SItemInGameTag::UPDATE_ItemTagBox()
{
	uint32 _X = FMath::FloorToInt32(m_ItemTagBorder->GetDesiredSize().X);
	uint32 _Y = FMath::FloorToInt32(m_ItemTagBorder->GetDesiredSize().Y);

	_X += ITEM_PADDING;
	_Y += ITEM_PADDING;

	m_InGameMenu->UPDATE_ItemTagBoxSize(_X, _Y);
	m_bNeedUpdate = false;
}

void SItemInGameTag::UPDATE_ItemTag(const FString& p_ItemName)
{
	if (m_ItemName)
	{
		m_ItemName->SetText(FText::FromString(p_ItemName));
		m_bNeedUpdate = true;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION