#include "Slate/Widgets/SGameMessageWidget.h"

DEFINE_LOG_CATEGORY(LogClass_SGameMessageWidget);

void SGameMessageWidget::Construct(const FArguments& p_OnArgs)
{
	m_HUDOwner = p_OnArgs._a_HUDOwner;

	ChildSlot
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Bottom)
	.Padding(0.0f, 0.0f, 0.0f, 80.0f)
	[
		SAssignNew(m_TextBlock, STextBlock)
		.Text(FText::FromString("The inventory is full !"))
		.Font(m_GeneralStyle.m_Monorita_Font)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(m_GeneralStyle.m_ErrorText_SlateColor)
	];
}

void SGameMessageWidget::SET_TextMessage(const FText& p_TextMessage)
{
	if (m_TextBlock == nullptr)
	{
		UE_LOG(LogClass_SGameMessageWidget, Warning, TEXT("SET_TextMessage() : m_TextBlock is NULL. || Unable to proceed further."));
		return;
	}

	m_TextBlock->SetText(p_TextMessage);
}