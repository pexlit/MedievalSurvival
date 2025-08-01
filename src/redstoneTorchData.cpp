#include "redstoneTorchData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
void redstoneTorchData::serializeMembers(nbtSerializer& s)
{
	attachmentDirectionData::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"last attached block power level"), lastAttachedBlockPowerLevel);
}

bool redstoneTorchData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	const powerLevel& newAttachedBlockPowerLevel = containerIn->getPowerLevel(position + directionVectors2D[(int)attachmentDirection]);
	if (lastAttachedBlockPowerLevel != newAttachedBlockPowerLevel)
	{
		lastAttachedBlockPowerLevel = newAttachedBlockPowerLevel;
		containerIn->addUpdatePosition(position);
	}
	return false;
}