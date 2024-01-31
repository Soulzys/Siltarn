#include "Interactables/PickupEntity.h"
#include "Siltarn/Public/Interactables/PickupActor.h"

DEFINE_LOG_CATEGORY(LogClass_UPickupEntity);

UPickupEntity::UPickupEntity()
{
	
}

UPickupEntity::~UPickupEntity()
{

}

void UPickupEntity::Initialize_Entity(APickupActor* p_IA)
{
	m_Name              = p_IA->GET_Name()              ; 
	m_Icon              = p_IA->GET_Icon()              ;
	m_InventorySpace    = p_IA->GET_InventorySpace()    ;
	m_ActorClass        = p_IA->GET_ItemClass()         ;
	m_ResellValue       = p_IA->GET_ResellValue()       ;
	m_Description       = p_IA->GET_Description()       ;
	m_Rarety            = p_IA->GET_Rarety()            ;
	m_bIsEquipable      = p_IA->GET_IsEquipable()       ;
	m_bIsDroppableAsIs  = p_IA->GET_IsDroppableAsIs()   ;
	m_DroppableBagClass = p_IA->GET_DroppableBagClass() ;

	check(m_Icon != nullptr);
}

EItemAnchorType UPickupEntity::GET_AnchorType() const
{
	int8 _X = GET_NumberType(m_InventorySpace.X);
	int8 _Y = GET_NumberType(m_InventorySpace.Y);

	// pp -> Cross
	if (_X == 0 && _Y == 0)
	{
		return EItemAnchorType::Cross;
	}

	// ii -> Tile
	if (_X == 1 && _Y == 1)
	{
		return EItemAnchorType::Tile;
	}

	// pi -> Line
	if (_X == 0 && _Y == 1)
	{
		return EItemAnchorType::Line;
	}

	// ip -> Line
	if (_X == 1 && _Y == 0)
	{
		return EItemAnchorType::Line;
	}

	return EItemAnchorType::Error;
}

bool UPickupEntity::IS_EntityUsable(bool p_bDisplayLogs)
{
	if (p_bDisplayLogs)
	{
		DEBUG_DisplayDataThroughUELogs();
	}

	bool _bIsUsable = m_Name.IsEmpty()                         ||
		              m_InventorySpace == FIntPoint::ZeroValue  ;

	return _bIsUsable;
}

uint8 UPickupEntity::GET_NumberType(uint32 p_N) const
{
	return p_N % 2;
}

EPickupEntityType UPickupEntity::GET_ItemType() const
{
	return EPickupEntityType::NotSet;
}

const FLinearColor& UPickupEntity::GET_RaretyColor() const
{
	uint8 _RaretyIndex = (uint8)m_Rarety;

	return m_RaretyTypesColors[_RaretyIndex];
}

void UPickupEntity::SET_ItemId(uint32 p_NewId)
{
	m_ItemId = p_NewId;
}

void UPickupEntity::SET_ControlTileId(uint32 p_NewId)
{
	m_ControlTileId = p_NewId;
}
