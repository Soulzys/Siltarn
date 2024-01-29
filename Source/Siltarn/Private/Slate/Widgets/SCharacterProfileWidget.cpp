#include "Slate/Widgets/SCharacterProfileWidget.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Interactables/EquipableEntity.h"
#include "Siltarn/Public/Interactables/InteractibleEnums.h"

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
			/*SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)*/

			SAssignNew(m_HeadItemWidget, SInventoryItemWidget)
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

		// Right Hand
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f - m_HandSlotSize.X, m_HeadSlotSize.Y + m_ChestSlotSize.Y + m_PantsSlotSize.Y + m_IntersticeSlotsDistance * 3.0f + 50.0f + 30.0f))
		.Size(m_HandSlotSize)
		[
			/*SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)*/

			SAssignNew(m_RightHandItemWidget, SInventoryItemWidget)
		]

		// Left Hand
		//
		+ SCanvas::Slot()
		.Position(FVector2D(m_CanvasSize.Get().X / 2.0f, m_HeadSlotSize.Y + m_ChestSlotSize.Y + m_PantsSlotSize.Y + m_IntersticeSlotsDistance * 3.0f + 50.0f + 30.0f))
		.Size(m_HandSlotSize)
		[
			SNew(SBorder)
			.BorderImage(&_GeneralStyle.m_DebuggingRed_SlateBrush)
			.Padding(0)
		]

		// Neck
		//
		+ SCanvas::Slot()
		.Position(FVector2D(50.0f + m_TileSize.Get() / 2.0f, 100.0f))
		.Size(m_NeckSlotSize)
		.Expose(m_NeckSlot)
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



	m_HeadItemWidget->SetVisibility(EVisibility::Hidden);
	m_RightHandItemWidget->SetVisibility(EVisibility::Hidden);
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
	m_NeckSlotSize      = FVector2D(_TileSize                           ); // Neck     -> 1x1 tiles
	m_ShouldersSlotSize = FVector2D(_TileSize * 2.0f                    );
	m_GlovesSlotSize    = FVector2D(_TileSize * 2.0f                    );
	m_BeltSlotSize      = FVector2D(_TileSize                           );
	m_ShoesSlotSize     = FVector2D(_TileSize * 2.0f                    );
	m_BagSlotSize       = FVector2D(_TileSize * 2.0f                    );
}

bool SCharacterProfileWidget::PLACE_ItemInSlot(SInventoryItemWidget* p_ItemWidget)
{
	if (p_ItemWidget == nullptr)
	{
		UE_LOG(LogClass_SCharacterProfileWidget, Error, TEXT("PLACE_ItemInSlot() : p_ItemWidget is NULL !"));
		return false;
	}

	UEquipableEntity* _ItemEntity = Cast<UEquipableEntity>(p_ItemWidget->GET_ItemData());

	if (_ItemEntity)
	{
		switch (_ItemEntity->GET_ItemEquipmentType())
		{
			case EItemEquipmentType::Head:
				UPDATE_HeadItemWidget(p_ItemWidget);
				break;

			case EItemEquipmentType::RightHand:
				UPDATE_RightHandWidget(p_ItemWidget);
				break;

			default:
				break;
		}
	}

	return false;
}

void SCharacterProfileWidget::UPDATE_HeadItemWidget(SInventoryItemWidget* p_ItemWidget)
{
	if (m_HeadItemWidget.IsValid() && p_ItemWidget)
	{
		m_HeadItemWidget->UPDATE_Widget(p_ItemWidget);
		m_HeadItemWidget->SetVisibility(EVisibility::Visible);
	}
}

void SCharacterProfileWidget::UPDATE_RightHandWidget(SInventoryItemWidget* p_ItemWidget)
{
	if (m_RightHandItemWidget.IsValid() && p_ItemWidget)
	{
		m_RightHandItemWidget->UPDATE_Widget(p_ItemWidget);
		m_RightHandItemWidget->SetVisibility(EVisibility::Visible);
	}
}