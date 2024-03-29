#include "Slate/Styles/SiltarnGeneralStyleContainer.h"

const FName FSiltarnGeneralStyleContainerStruct::TypeName = TEXT("FSiltarnGeneralStyleContainerStruct");

void FSiltarnGeneralStyleContainerStruct::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&m_Inventory_SlateBrush            );
	OutBrushes.Add(&m_InventoryItemIcon_SlateBrush    );
	OutBrushes.Add(&m_DebuggingBlue_SlateBrush        );
	OutBrushes.Add(&m_DebuggingRed_SlateBrush         );
	OutBrushes.Add(&m_DebuggingGreen_SlateBrush       );
	OutBrushes.Add(&m_DebuggingCyan_SlateBrush        );
	OutBrushes.Add(&m_DebuggingYellow_SlateBrush      );
	OutBrushes.Add(&m_DebuggingMagenta_SlateBrush     );
	OutBrushes.Add(&m_WhiteBackgroundImage_SlateBrush );
	OutBrushes.Add(&m_ItemTooltipBorder_SlateBrush    );
	OutBrushes.Add(&m_ItemBagIcon_SlateBrush          );
	OutBrushes.Add(&m_Alphabet_E_SlateBrush           );
	OutBrushes.Add(&m_Box_Alphabet_SlateBrush         );
}

const FSiltarnGeneralStyleContainerStruct& FSiltarnGeneralStyleContainerStruct::GetDefault()
{
	static FSiltarnGeneralStyleContainerStruct _DefaultStruct;
	return _DefaultStruct;
}

