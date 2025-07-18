#include "craftingTableSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
craftingTableSlotContainer::craftingTableSlotContainer() : craftingGridSlotContainer(new uiSlotContainer(cveci2(30, 97), new rectangularSlotContainer(cveci2(3))))
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / L"crafting_table.png");
	containers.push_back(craftingSlots);
	containers.push_back(outputSlot = new uiSlotContainer(cveci2(125, 115), new rectangularSlotContainer(cveci2(1))));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool craftingTableSlotContainer::addToEqualStacks(itemStack& s, itemStack*& emptySlot)
{
	return craftingSlots->addToEqualStacks(s, emptySlot);
}

craftingTableSlotContainer::~craftingTableSlotContainer()
{
	delete inventorySlots;
	delete hotbarSlots;

	delete craftingSlots;

	delete outputSlot;
}