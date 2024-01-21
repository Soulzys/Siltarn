#include "Inventory/Inventory.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"

DEFINE_LOG_CATEGORY(LogClass_UInventory);

uint32 UInventory::m_InstanceCount = 0;

UInventory::UInventory()
{
	m_InstanceCount++;
	UE_LOG(LogClass_UInventory, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

UInventory::~UInventory()
{
	m_InstanceCount--;
	UE_LOG(LogClass_UInventory, Log, TEXT("An instance was destroyed ! || Instance count : %d"), m_InstanceCount);
}

void UInventory::SET_SiltarnPlayerController(ASiltarnPlayerController* p_Ref)
{
	checkf(p_Ref != nullptr, TEXT("UInventory::SET_GameplayHUD() : p_Ref is NULL."));

	m_PlayerController = p_Ref;
}

void UInventory::ADD_Item(UPickupEntity* p_Item, bool p_bDisplayLogs)
{
	checkf(p_Item != nullptr, TEXT("UInventory::ADD_Item() : p_Item is NULL !"));

	int32 _NewId = CREATE_NewItemId();
	p_Item->SET_ItemId(_NewId);
	m_Items.Add(p_Item);

	if (p_bDisplayLogs)
	{
		DEBUG_DisplayItemDataThroughUELogs(p_Item->GET_ItemId());
	}	
}

void UInventory::REMOVE_Item(UPickupEntity* p_Item, bool p_bDisplayLogs)
{
	checkf(p_Item != nullptr, TEXT("UInventory::REMOVE_Item() : p_Item is NULL !"));

	if (p_bDisplayLogs)
	{
		DEBUG_DisplayItemDataThroughUELogs(p_Item->GET_ItemId());
	}

	for (int32 i = 0; i < m_Items.Num(); i++)
	{
		if (m_Items[i]->GET_ItemId() == p_Item->GET_ItemId())
		{
			m_Items.RemoveAt(i);
			return;
		}		
	}

	check(p_Item);
	p_Item->BeginDestroy();
}

int32 UInventory::CREATE_NewItemId()
{
	if (m_Items.IsEmpty())
	{
		return 0;
	}

	/*
		This is not ideal at all. We need to find a way to do it more efficiently, not have to potentially go through each elements of the array twice
	*/
	m_Items.Sort();

	int32 _NewId = 0;

	for (int32 i = 0; i < m_Items.Num(); i++)
	{
		int32 _CurrentItemId = m_Items[i]->GET_ItemId();

		if (i < _CurrentItemId)
		{
			return i;
		}
	}

	return m_Items.Num();
}

void UInventory::DEBUG_DisplayItemDataThroughUELogs(int32 p_ItemId)
{
	for (int32 i = 0; i < m_Items.Num(); i++)
	{
		if (m_Items[i]->GET_ItemId() == p_ItemId)
		{
			m_Items[i]->DEBUG_DisplayDataThroughUELogs();
			return;
		}
	}
}

void UInventory::DEBUG_DisplayAllItemsDataThroughUELogs()
{
	for (UPickupEntity* _Entity : m_Items)
	{
		_Entity->DEBUG_DisplayDataThroughUELogs();
	}
}