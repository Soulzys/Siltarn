#include "External/Documentation.h"

/*
	Drag and drop item in inventory

	1/ Click on item

	2/ Drag item to desired FTile

	3/ Release the drag button

		a) if (FTile is occupied or cannot fit the item)
			-> position the item back to the FTile it originally was before the drag and drop

		b) if (FTile is out of the inventory)
			-> remove the item from the inventory and destroys its widget
			-> spawns the item in the world so it can be picked up once more

		c) if (FTile can welcome the item)
			->

*/


/*
	THINGS TO KNOW

	Calling Empty() on a TArray, a TMap... where objects are stored does not destroy such objects, including UObject types.
*/




/*
	1/ Find largest item in queue
	2/ Set the new item location tile
	3/ Set the inventory size from the item size
	4/ Build tiles, set them to occupied and store them in a TMap where a key is a tile coordinates and the value is the tile itself
	5/ Remove the item from the queue and store it into the bag items TArray
	6/ While there are still items in queue, do..
		6.1/ Get the largest item in queue
		6.2/ Set the item new location tile
		6.3/ Build tiles for item
		6.4/
*/



/*
	THINGS TO NEVER DO

	
	1/ Never ever delete a TArray element within a FOR LOOP that is going through the TArray.
	
	Example : for (int32 i = 0; i < arr.Num(); i++)
			  {
			      arr.RemoveAt(i); // or arr.RemoveAt(i + 1) or arr.RemoveAt(i - 1) etc
			  }



	One fix for this issue is to use a WHILE LOOP instead of a FOR LOOP

	Example : int _Counter = 0;
			  while (_Counter < arr.Num())
			  {
				  if (condition)
				  {
				      arr.RemoveAt(_Counter);
				  }
				  else
				  {
					  _Counter++;
				  }
			  }
*/




/*
	Notes brouillon
	---------------

	At this point, SInventoryItemWidget contains : 
	- UPickupEntity*
	- item size
	- SInventoryWidget*
	- EInventoryItemWidgetLocation
	- All the occupied FTile (the first of the array being the control tile)

	+ UPickupEntity::m_InventoryLocationTile has been SET

	+ SInventoryWidget::m_CachedTileIndex and SInventoryWidget::m_CachedTilesIndexes have been RESET


	
*/




/*
	Adding item to player inventory from world (14/04/2024)

	1/ 
	APickupActor::INTERFACE_Interact( ASiltarnPlayerController* )
		a) Checks if there is enough space in the inventory. 
				- if enough space     : sets SInventoryWidget::m_CachedTileIndex (*1) and SInventoryWidget::m_CachedTilesIndexes
				- if not enough space : leaves the function -> the item is not picked up
		b) Creates a new UPickupEntity object

	2/ 
	UInventoryManager::AddItemToPlayerInventory( UPickupEntity* )
		a) Use
		




	##### 

	(*1) We may not need m_CachedTileIndex and probably should only retrieve m_CachedTilesIndexes[0] as a substitute.

*/