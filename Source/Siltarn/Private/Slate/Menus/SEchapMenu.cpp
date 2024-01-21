#include "Slate/Menus/SEchapMenu.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

DEFINE_LOG_CATEGORY(LogClass_SEchapMenu);

void SEchapMenu::Construct(const FArguments& p_InArgs)
{
	m_HUDOwner = p_InArgs._a_HUDOwner;

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SBorder)
		.BorderImage(&m_GeneralStyle.m_WhiteBackgroundImage_SlateBrush)
		.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.35f))
		.Padding(0)
		[
			SNew(SBorder)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(20.0f)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SNew(SButton)
					.OnClicked(this, &SEchapMenu::QUIT_Game)
					.ContentPadding(10.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Exit game"))
						.Justification(ETextJustify::Center)
					]
					
				]	

				+ SVerticalBox::Slot()
				[
					SNew(SButton)
					.OnClicked(this, &SEchapMenu::CLOSE_Menu)
					.ContentPadding(10.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Resume game"))
						.Justification(ETextJustify::Center)
					]
				]
			]			
		]
	];
}

FReply SEchapMenu::QUIT_Game()
{
	APlayerController* _PC = m_HUDOwner->GetOwningPlayerController();

	if (_PC)
	{
		_PC->ConsoleCommand("quit");
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SEchapMenu::CLOSE_Menu()
{
	if (m_HUDOwner.IsValid())
	{
		ASiltarnPlayerController* _SPC = Cast<ASiltarnPlayerController>(m_HUDOwner->GetOwningPlayerController());

		if (_SPC)
		{
			_SPC->TOGGLE_EchapMenu();
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION