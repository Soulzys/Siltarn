#include "Interactables/PickupEntity.h"
#include "Siltarn/Public/Interactables/PickupActor.h"

DEFINE_LOG_CATEGORY(LogClass_UPickupEntity);

UPickupEntity::UPickupEntity()
{
	m_EntityId = -1;

	UE_LOG(LogClass_UPickupEntity, Error, TEXT("WTF IS GOING ON ??"));
	UE_LOG(LogClass_UPickupEntity, Warning, TEXT("Inventory space : %s"), *m_InventorySpace.ToString());
	UE_LOG(LogClass_UPickupEntity, Warning, TEXT("Entity id : %lld"), m_EntityId);
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

	m_EntityId = ComputeUniqueId();

	UE_LOG(LogClass_UPickupEntity, Warning, TEXT("Initialize_Entity() --> m_EntityId = %lld "), m_EntityId);
	UE_LOG(LogTemp, Error, TEXT("Initialize_Entity() was called !"));

	// Luciole 28/03/2024 || Need to better manage this check
	check(m_Icon != nullptr);
}

int64 UPickupEntity::ComputeUniqueId()
{
	FDateTime _DateTime       = FDateTime::Now()           ;
	FString   _DateTimeString = _DateTime.ToString()       ;
	int32     _Milliseconds   = _DateTime.GetMillisecond() ; // int from 0 to 999
	int32     _FillingMilli   = 0                          ;

	// Not very optimal but well..
	if (_Milliseconds < 100)
	{
		_FillingMilli = FMath::RandRange(1, 9);

		if (_Milliseconds < 10)
		{
			_FillingMilli = FMath::RandRange(10, 99);
		}
	}

	_DateTimeString.RemoveAt(10)                                   ; // _DateTime has yyyy.mm.dd-hh.mm.ss format. Here, we remove the '-' char.
	_DateTimeString = _DateTimeString.Replace(TEXT("."), TEXT("")) ; // We then end up with a FString only comprised of numbers, which can be casted to an int
	_DateTimeString.Append(FString::FromInt(_Milliseconds))        ;

	if (_FillingMilli > 0) // If _Milliseconds was not a three digits number and therefore got processed through the double if () statement above
	{
		_DateTimeString.Append(FString::FromInt(_FillingMilli));
	}

	int8 _R1 = FMath::RandRange(1, 9); // If the first char of an FString is 0 and said FString is then converted to an integer, the 0 will be thrown away
	int8 _R2 = FMath::RandRange(0, 9);
	int8 _R3 = FMath::RandRange(0, 9);

	// See Documentation.h
	_DateTimeString[0] = FString::FromInt(_R1)[0];
	_DateTimeString[1] = FString::FromInt(_R2)[0];
	_DateTimeString.Append(FString::FromInt(_R3));


	return FCString::Atoi64(*_DateTimeString);
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

void UPickupEntity::SET_InventoryId(int32 p_NewId)
{
	m_InventoryId = p_NewId;
}

void UPickupEntity::SET_InventoryLocationTile(const FIntPoint p_OriginTileCoordinates)
{
	m_InventoryLocationTile = p_OriginTileCoordinates;
}