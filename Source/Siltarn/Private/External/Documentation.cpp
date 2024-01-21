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