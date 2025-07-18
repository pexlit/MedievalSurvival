#include "mobData.h"
#include "creeper.h"
#include "taskList.h"
#include "explodeAI.h"
#include "wanderAI.h"
#include "attackOnCollideAI.h"
#include "nbt/nbtSerializer.h"
creeper::creeper() :mob(entityID::creeper), fourLeggedMob(entityID::creeper)
{
	tasks = new taskList(this, {
	new explodeAI(this, entityID::human),
	new attackOnCollideAI(this, entityID::human),
	new wanderAI(this), });

	//intialize body parts
	mainBodyPart = new bodyPart2D(creeperBodyTextureRect, nullptr, vec2(position.x, position.y + creeperLegSize.y), creeperBodySize, creeperBodyRotationCenter);

	frontRightLeg = new bodyPart2D(creeperFrontRightLegTextureRect, mainBodyPart, creeperFrontLegOffset, creeperLegSize, creeperFrontLegRotationCenter);
	frontLeftLeg = new bodyPart2D(creeperFrontLeftLegTextureRect, mainBodyPart, creeperFrontLegOffset, creeperLegSize, creeperFrontLegRotationCenter);

	backRightLeg = new bodyPart2D(creeperFrontRightLegTextureRect, mainBodyPart, creeperBackLegOffset, creeperLegSize, creeperBackLegRotationCenter);
	backLeftLeg = new bodyPart2D(creeperFrontLeftLegTextureRect, mainBodyPart, creeperBackLegOffset, creeperLegSize, creeperBackLegRotationCenter);

	head = new bodyPart2D(creeperHeadTextureRect, mainBodyPart, vec2(0, creeperBodySize.y), creeperHeadSize, creeperHeadRotationCenter);
	connectBodyParts();

}

void creeper::render(const gameRenderData& targetData) const
{
	if ((ticksUntilFuse < creeperFuseLength) && (ticksUntilFuse % ticksPerRealLifeSecond < (ticksPerRealLifeSecond / 2)))
	{
		updateBodyParts();
		//render white
		const solidColorBrush flashColorBrush = solidColorBrush(color(color::halfMaxValue, color::maxValue, color::maxValue, color::maxValue));
		mat3x3 transform = targetData.worldToRenderTargetTransform;//TODO: antialiasing for creeper skin
		auto flashingBrush = colorMixer<solidColorBrush, resolutionTexture>(flashColorBrush, *((mobData*)entityDataList[(int)entityType])->skin);

		renderBodyPart(mainBodyPart, transform, flashingBrush, targetData);
	}
	else
	{
		fourLeggedMob::render(targetData);
	}
}

void creeper::serializeMembers(nbtSerializer& s)
{
	fourLeggedMob::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"powered"), powered);
	serializeNBTValue(s, std::wstring(L"explosion strength"), explosionStrength);
	serializeNBTValue(s, std::wstring(L"ticks until fuse"), ticksUntilFuse);
	serializeNBTValue(s, std::wstring(L"ignited"), ignited);
}