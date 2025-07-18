#pragma once
#include "sapSpreadData.h"
#include "pollenData.h"
struct leavesData : sapSpreadData, pollenData
{
	virtual void serializeMembers(nbtSerializer& s) override {
		sapSpreadData::serializeMembers(s);
	}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override {
		pollenData::randomTick(containerIn, position);
		sapSpreadData::randomTick(containerIn, position);
	}
};