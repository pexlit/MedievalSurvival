#pragma once
#include "humanoid.h"
//skeleton

constexpr rectangle2 skeletonBodyTextureRect = crectangle2(16, 0, 4, 12);
constexpr rectangle2 skeletonHeadTextureRect = crectangle2(0, 16, 8, 8);
constexpr rectangle2 skeletonRightLegTextureRect = crectangle2(0, 2, 2, 12);
constexpr rectangle2 skeletonLeftLegTextureRect = crectangle2(0, 2, 2, 12);
constexpr rectangle2 skeletonRightArmTextureRect = crectangle2(40, 2, 2, 12);
constexpr rectangle2 skeletonLeftArmTextureRect = crectangle2(44, 2, 2, 12);

constexpr vec2 skeletonArmSize = cvec2(skeletonLeftLegTextureRect.size) * humanoidPixelSize;

struct skeleton : public humanoid
{
	skeleton(const entityID& entityType);
	virtual void serializeMembers(nbtSerializer& s) override;
};