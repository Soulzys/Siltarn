#include "Slate/Widgets/SInventoryBagWidget.h"
//#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInGameBagInventory.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"

DEFINE_LOG_CATEGORY(LogClass_SInventoryBagWidget);

void SInventoryBagWidget::Construct(const FArguments& p_InArgs)
{
	const FSiltarnGeneralStyleContainerStruct& _GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();

	m_HUDOwner = p_InArgs._a_HUDOwner;

	ChildSlot
	[
		SNew(SCanvas)
		
		+ SCanvas::Slot()
		.Position(FVector2D(200.0f, 300.0f))
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingBlue_SlateBrush)
			.Padding(0)
			[
				SNew(SBox)
				.WidthOverride(8 * 75.0f)
				.HeightOverride(5 * 75.0f)
				[
					SAssignNew(m_InventoryWidget, SInGameBagInventory)
					.a_NumberOfColumns(8)
					.a_NumberOfRows(5)
					.a_TileSize(75.0f)
					.a_HUDOwner(m_HUDOwner)
					.a_ParentWidget(nullptr)
				]
			]
		]		
	];
}