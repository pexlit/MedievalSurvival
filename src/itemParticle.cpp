#include "itemParticle.h"
#include "itemData.h"
#include "nbt/nbtSerializer.h"

const resolutionTexture& itemParticle::getTexture() const
{
	return *itemList[brushItemID]->tex;
}

void itemParticle::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"brush item id"), brushItemID);
	rectParticle::serializeMembers(s);
}