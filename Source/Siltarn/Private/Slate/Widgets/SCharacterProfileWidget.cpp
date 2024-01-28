#include "Slate/Widgets/SCharacterProfileWidget.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"

DEFINE_LOG_CATEGORY(LogClass_SCharacterProfileWidget);

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCharacterProfileWidget::Construct(const FArguments& p_InArgs)
{
	const FSiltarnGeneralStyleContainerStruct& _GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();

	m_CanvasSize = p_InArgs._a_CanvasSize;
	m_ItemSlotSize = p_InArgs._a_ItemSlotSize;
	m_TileSize = p_InArgs._a_TileSize;

	m_IntersticeSlotsDistance = 20.0f;

	COMPUTE_SlotsSizes();

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SCanvas)
		
		// Head
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f - m_HeadSlotSize.X / 2.0f, 50.0f))
		.Size(m_HeadSlotSize)
		.Expose(m_HeadSlot)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Chest
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f - m_ChestSlotSize.X / 2.0f, m_HeadSlotSize.Y + m_IntersticeSlotsDistance + 50.0f))
		.Size(m_ChestSlotSize)
		.Expose(m_ChestSlot)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Pants
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f - m_PantsSlotSize.X / 2.0f, m_HeadSlotSize.Y + m_ChestSlotSize.Y + m_IntersticeSlotsDistance * 2.0f + 50.0f))
		.Size(m_PantsSlotSize)
		.Expose(m_PantsSlot)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Hand 1
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f - m_HandSlotSize.X, m_HeadSlotSize.Y + m_ChestSlotSize.Y + m_PantsSlotSize.Y + m_IntersticeSlotsDistance * 3.0f + 50.0f + 30.0f))
		.Size(m_HandSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Hand 2
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f, m_HeadSlotSize.Y + m_ChestSlotSize.Y + m_PantsSlotSize.Y + m_IntersticeSlotsDistance * 3.0f + 50.0f + 30.0f))
		.Size(m_HandSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Necklace
		//
		+ SCanvas::Slot()
		.Position(FVector2D(50.0f + m_TileSize.Get() / 2.0f, 100.0f))
		.Size(m_NecklaceSlotSize)
		.Expose(m_NecklaceSlot)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Shoulders
		//
		+ SCanvas::Slot()
		.Position(FVector2D(50.0f, 200.0f))
		.Size(m_ShouldersSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Gloves
		//
		+ SCanvas::Slot()
		.Position(FVector2D(50.0f, 350.0f))
		.Size(m_GlovesSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Belt
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X - m_BeltSlotSize.X - m_TileSize.Get() / 2.0f - 50.0f, 100.0f))
		.Size(m_BeltSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Shoes
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X - m_ShoesSlotSize.X - 50.0f, 200.0f))
		.Size(m_ShoesSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Bag
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X - m_BagSlotSize.X - 50.0f, 350.0f))
		.Size(m_BagSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCharacterProfileWidget::COMPUTE_SlotsSizes()
{
	float _TileSize = m_TileSize.Get();

	// Luciole 28/01/2024 | Many calculations are alike. We do this now because we may change some item slot's proportion later. 
	// When all items slots' proportions are set in stone, we should probably change these to, for instance, m_2x2Slot, m_3x2Slot, etc
	m_HeadSlotSize      = FVector2D(_TileSize * 2.0f                    ); // Head     -> 2x2 tiles
	m_ChestSlotSize     = FVector2D(_TileSize * 3.0f                    ); // Chest    -> 3x3 tiles
	m_PantsSlotSize     = FVector2D(_TileSize * 2.0f , _TileSize * 3.0f ); // Pants    -> 2x3 tiles
	m_HandSlotSize      = FVector2D(_TileSize * 2.0f                    ); // Hand     -> 2x2 tiles
	m_NecklaceSlotSize  = FVector2D(_TileSize                           ); // Necklace -> 1x1 tiles
	m_ShouldersSlotSize = FVector2D(_TileSize * 2.0f                    );
	m_GlovesSlotSize    = FVector2D(_TileSize * 2.0f                    );
	m_BeltSlotSize      = FVector2D(_TileSize                           );
	m_ShoesSlotSize     = FVector2D(_TileSize * 2.0f                    );
	m_BagSlotSize       = FVector2D(_TileSize * 2.0f                    );
}