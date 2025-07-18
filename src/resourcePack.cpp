#include "structureID.h"
#include "noteTypeID.h"
#include "gameModeData.h"
#include "statusEffectData.h"
#include "statusEffectID.h"
#include "dimensionID.h"
#include "biomeID.h"
#include "dimensionData.h"
#include "itemID.h"
#include "idList.h"
#include "enchantmentData.h"
#include "resourcePack.h"
#include "noteData.h"
#include "gameModeID.h"
#include "structureData.h"
#include "mushroomColorData.h"
#include "netherVineTypeData.h"
#include "woodTypeData.h"
#include "particleTypeData.h"
#include "fireworkShapeData.h"
#include "structure.h"
#include "filesystem/textfile.h"
#include "furnaceRecipe.h"
#include "recipe.h"
#include "entityData.h"
#include "block.h"
#include "audioCollection.h"
#include "fireworkShapeID.h"
#include "netherVineTypeID.h"
#include "textureList.h"
#include "folderList.h"
#include "colors.h"
#include "musicList.h"
#include "soundList.h"
#include "oreBlockList.h"
#include "coralStateNames.h"
#include "coralColorNames.h"
#include "coralTypeNames.h"
#include "doubleBlock.h"
#include "connectedBlock.h"
#include "sandstoneTypeID.h"
#include "cropBlock.h"
#include "itemData.h"
#include "mobData.h"
#include "enderman.h"
#include "slime.h"
#include "pig.h"
#include "wolf.h"
#include "sheep.h"
#include "chicken.h"
#include "cow.h"
#include "blaze.h"
#include "ghast.h"
#include "enderDragon.h"
#include "creeper.h"
#include "cod.h"
#include "shulker.h"
#include "minecart.h"
#include "experienceOrb.h"
#include "particle.h"
#include "throwableData.h"
#include "endCrystal.h"
#include "pollen.h"
#include "tag.h"

#include "lootTable.h"
#include "idConverter.h"
#include "jigsawPool.h"
#include "renderBrewingStand.h"
#include <math.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "armorTier.h"
#include "armorType.h"
#include "biomeData.h"
#include "collisionTypeID.h"
#include "colorID.h"
#include "constants.h"
#include "coralColorID.h"
#include "coralStateID.h"
#include "coralTypeID.h"
#include "enchantmentID.h"
#include "entity.h"
#include "entityID.h"
#include "experienceDrop.h"
#include "harvestTier.h"
#include "harvestType.h"
#include "humanoid.h"
#include "idAnalysis.h"
#include "array/fastlist.h"
#include "array/wstringFunctions.h"
#include "filesystem/jsonReader.h"
#include "globalFunctions.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/color/color.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/graphics/resolutiontexture.h"
#include "math/wave/waveShaper.h"
#include "math/graphics/texture.h"
#include "math/vector/vectn.h"
#include "levelID.h"
#include "lightLevel.h"
#include "mushroomColorID.h"
#include "particleID.h"
#include "sandstoneColorID.h"
#include "treeItemTypeID.h"
#include "woodtypeID.h"
#include "minecraftFont.h"
#include "include/math/graphics/brush/brushes/colorMultiplier.h"
#include "include/math/graphics/brush/brushes/solidColorBrush.h"
#include "include/math/graphics/graphicsFunctions.h"
#include "resourceLoader.h"
#include <Trades.h>

minecraftFont* currentMinecraftFont = nullptr;
fontFamily* currentMinecraftFontFamily = nullptr;

std::vector<resolutionTexture*> loadedTextures = std::vector<resolutionTexture*>();
std::wstring creditsText = std::wstring();

const std::wstring musicDiscNames[musicDiscTypeCount]{
	std::wstring(L"13"),
	std::wstring(L"cat"),
	std::wstring(L"blocks"),
	std::wstring(L"chirp"),
	std::wstring(L"far"),
	std::wstring(L"mall"),
	std::wstring(L"mellohi"),
	std::wstring(L"stal"),
	std::wstring(L"strad"),
	std::wstring(L"ward"),
	std::wstring(L"11"),
	std::wstring(L"wait"),
	std::wstring(L"pigstep") };

const std::wstring oreBlockNames[oreBlockTypeCount]{
	std::wstring(L"coal"),
	std::wstring(L"iron"),
	std::wstring(L"emerald"),
	std::wstring(L"lapis"),
	std::wstring(L"redstone"),
	std::wstring(L"gold"),
	std::wstring(L"diamond"),
	std::wstring(L"nether_quartz"),
	std::wstring(L"nether_gold") };

constexpr bool smeltable[oreBlockTypeCount]{
	false,
	true,
	false,
	false,
	false,
	true,
	false,
	false,
	false };

// PER ARMOR TYPE
constexpr fp leatherArmorWeakness[armorTypeCount]{
	1.0 / 65,
	1.0 / 75,
	1.0 / 80,
	1.0 / 55 };

constexpr fp armorTierWeaknessMultiplier[armorTierCount]{
	1.0 / 5,
	1,
	1.0 / 1.5,
	1.0 / 3,
	1.0 / 3,
	1.0 / 6,
	1.0 / 7,
};

// PER ARMOR TIER
constexpr fp toolWeakness[toolTierCount]{
	1.0 / 0x40,
	1.0 / 0x20,
	1.0 / 0x80,
	1.0 / 0x100,
	1.0 / 0x600,
	1.0 / 0x800 };

constexpr int armorEnchantability[armorTierCount]{
	15,
	12,
	9,
	25,
	10,
	9,
	15,
};
constexpr int toolEnchantability[toolTierCount]{
	15,
	5,
	14,
	22,
	10,
	15,
};



idList<block*, blockID>
blockList = idList<block*, blockID>();
idList<itemData*, itemID> itemList = idList<itemData*, itemID>();
idList<dimensionData*, dimensionID> dimensionDataList = idList<dimensionData*, dimensionID>();
idList<biomeData*, biomeID> biomeDataList = idList<biomeData*, biomeID>();
idList<enchantmentData*, enchantmentID> enchantmentDataList = idList<enchantmentData*, enchantmentID>();
idList<statusEffectData*, statusEffectID> statusEffectDataList = idList<statusEffectData*, statusEffectID>();
idList<gameModeData*, gameModeID> gameModeDataList = idList<gameModeData*, gameModeID>();
idList<noteData*, noteTypeID> noteDataList = idList<noteData*, noteTypeID>();
idList<structureData*, structureID> structureDataList = idList<structureData*, structureID>();
idList<mushroomColorData*, mushroomColorID> mushroomColorDataList = idList<mushroomColorData*, mushroomColorID>();
idList<netherVineTypeData*, netherVineTypeID> netherVineTypeDataList = idList<netherVineTypeData*, netherVineTypeID>();
idList<woodTypeData*, woodTypeID> woodTypeDataList = idList<woodTypeData*, woodTypeID>();
idList<particleTypeData*, particleID> particleTypeDataList = idList<particleTypeData*, particleID>();
idList<fireworkShapeData*, fireworkShapeID> fireworkShapeDataList = idList<fireworkShapeData*, fireworkShapeID>();

resolutionTexture* loadRotatedTexture(const stdPath& path, cvec2& defaultSize, cint& angle)
{
	resolutionTexture* originalImage = loadTextureFromResourcePack(path, false);
	texture rotatedImage(cveci2(blockTextureSize), false);
	fillTransformedTexture(mat3x3::rotate(cvec2(blockTextureSize * 0.5), angle), *originalImage, rotatedImage);

	delete originalImage;
	resolutionTexture* rotatedTex = new resolutionTexture(rotatedImage);
	loadedTextures.push_back(rotatedTex);
	return rotatedTex;
}

static resolutionTexture* loadDragonEggTexture()
{
	resolutionTexture* dragonEggSurfaceTexture = loadTextureFromResourcePack(blockTextureFolder / L"dragon_egg.png");
	// resolutionTexture *dragonEggGraphics = new resolutionTexture(texture(cvect2<fsize_t>(dragonEggSurfaceTexture->scaledTextures[0]->size)), cvec2(blockTextureSize));
	//  fill oval which is cut off at the bottom

	cfp cutOffPart = 0.2;
	const texture& dragonEggGraphics = texture(cvect2<fsize_t>(dragonEggSurfaceTexture->scaledTextures[0]->size));
	rectangle2 dragonEggTextureRect = crectangle2(0, dragonEggGraphics.size.x * -cutOffPart, dragonEggGraphics.size.x, dragonEggGraphics.size.y * (1 + cutOffPart));

	fillEllipse(dragonEggGraphics, dragonEggTextureRect, *dragonEggSurfaceTexture->scaledTextures[0]);
	auto dragonEggTex = new resolutionTexture(dragonEggGraphics, vec2(blockTextureSize));
	loadedTextures.push_back(dragonEggTex);
	return dragonEggTex;
}

resolutionTexture* loadRailsTexture(const stdPath& path)
{
	resolutionTexture* tex = loadTextureFromResourcePack(path, false);
	texture renderTarget(cvect2<fsize_t>((fsize_t)blockTextureSize), false);

	constexpr rectangle2 unTransformedMiddleTextureRect = crectangle2(7, 0, 1, 0x10);
	constexpr rectangle2 unTransformedRailsTextureRect = crectangle2(2, 0, 4, 0x10);
	constexpr rectangle2 unTransformedSleepersTextureRect = crectangle2(14, 0, 1, 0x10);

	cmat3x3 middleTransform = mat3x3::fromRectToRotatedRect(crectangle2(unTransformedMiddleTextureRect), 90, crectangle2(middleTextureRect));
	cmat3x3 railsTransform = mat3x3::fromRectToRotatedRect(crectangle2(unTransformedRailsTextureRect), 90, crectangle2(railsTextureRect));
	cmat3x3 sleepersTransform = mat3x3::fromRectToRotatedRect(crectangle2(unTransformedSleepersTextureRect), 90, crectangle2(sleepersTextureRect));
	fillTransparentRectangle(crectangle2(unTransformedMiddleTextureRect), middleTransform, *tex, renderTarget);
	fillTransparentRectangle(crectangle2(unTransformedRailsTextureRect), railsTransform, *tex, renderTarget);
	fillTransparentRectangle(crectangle2(unTransformedSleepersTextureRect), sleepersTransform, *tex, renderTarget);
	delete tex;
	resolutionTexture* railsTex = new resolutionTexture(renderTarget, cvec2(blockTextureSize));
	loadedTextures.push_back(railsTex);
	return railsTex;
}
resolutionTexture* loadTextureFromResourcePack(const stdPath& relativePath, cbool& addToTextureList)
{
	const auto& locations = getResourceLocations(relativePath);
	if (locations.size() == 0)
	{
		handleError(relativePath.wstring() + std::wstring(L" not found in any of the resource packs. working directory: ") + workingDirectory.wstring());
	}
	csize_t& lastLocation = locations.size() - 1;
	veci2 size = getImageSize(locations[0]); // the base size of the image will be the one of the lowest resource pack: the minecraft texture folder or the "default" resource pack.

	return loadTexture(locations[lastLocation], size, addToTextureList);
}

resolutionTexture* loadTexture(stdPath path, cvec2& defaultSize, cbool& addToTextureList)
{
	resolutionTexture* const& result = new resolutionTexture(texture(path, true), defaultSize);
	if (addToTextureList)
	{
		loadedTextures.push_back(result);
	}
	return result;
}

static void loadDataLists()
{
	enchantmentDataList = idList<enchantmentData*, enchantmentID>(fastList<enchantmentData*>((int)enchantmentID::count));
	int enchantmentId = 0;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"aqua_affinity"), (enchantmentID)enchantmentId, 1, { {1, 41} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"bane_of_arthropods"), (enchantmentID)enchantmentId, 5, { {5, 25}, {13, 33}, {21, 41}, {29, 49}, {37, 57} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"blast_protection"), (enchantmentID)enchantmentId, 4, { {5, 13}, {13, 21}, {21, 29}, {29, 37} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"channeling"), (enchantmentID)enchantmentId, 1, { {25, 50} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"curse_of_binding"), (enchantmentID)enchantmentId, 1, { {25, 50} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"curse_of_vanishing"), (enchantmentID)enchantmentId, 1, { {25, 50} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"depth_strider"), (enchantmentID)enchantmentId, 3, { {10, 25}, {20, 35}, {30, 45} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"efficiency"), (enchantmentID)enchantmentId, 5, { {1, 61}, {11, 71}, {21, 81}, {31, 91}, {41, 101} }, 10);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"feather_falling"), (enchantmentID)enchantmentId, 4, { {5, 11}, {11, 17}, {17, 23}, {23, 29} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"fire_aspect"), (enchantmentID)enchantmentId, 2, { {10, 61}, {30, 71} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"fire_protection"), (enchantmentID)enchantmentId, 4, { {10, 18}, {18, 26}, {26, 34}, {34, 42} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"flame"), (enchantmentID)enchantmentId, 1, { {20, 50} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"fortune"), (enchantmentID)enchantmentId, 3, { {15, 61}, {24, 71}, {33, 81} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"frost_walker"), (enchantmentID)enchantmentId, 2, { {10, 25}, {20, 35} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"impaling"), (enchantmentID)enchantmentId, 5, { {1, 21}, {9, 29}, {17, 37}, {25, 45}, {33, 53} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"infinity"), (enchantmentID)enchantmentId, 1, { {20, 50} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"knockback"), (enchantmentID)enchantmentId, 2, { {5, 61}, {25, 71} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"looting"), (enchantmentID)enchantmentId, 3, { {15, 61}, {24, 71}, {33, 81} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"loyalty"), (enchantmentID)enchantmentId, 3, { {12, 50}, {19, 50}, {26, 50} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"luck_of_the_sea"), (enchantmentID)enchantmentId, 3, { {15, 61}, {24, 71}, {33, 81} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"lure"), (enchantmentID)enchantmentId, 3, { {15, 61}, {24, 71}, {33, 81} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"mending"), (enchantmentID)enchantmentId, 1, { {25, 75} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"multishot"), (enchantmentID)enchantmentId, 1, { {20, 50} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"piercing"), (enchantmentID)enchantmentId, 4, { {1, 50}, {11, 50}, {21, 50}, {31, 50} }, 10);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"power"), (enchantmentID)enchantmentId, 5, { {1, 16}, {11, 26}, {21, 36}, {31, 46}, {41, 56} }, 10);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"projectile_protection"), (enchantmentID)enchantmentId, 4, { {3, 9}, {9, 15}, {15, 21}, {21, 27} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"protection"), (enchantmentID)enchantmentId, 4, { {1, 12}, {12, 23}, {23, 34}, {34, 45} }, 10);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"punch"), (enchantmentID)enchantmentId, 2, { {12, 37}, {32, 57} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"quick_charge"), (enchantmentID)enchantmentId, 3, { {12, 50}, {32, 50}, {52, 50} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"respiration"), (enchantmentID)enchantmentId, 3, { {10, 40}, {20, 50}, {30, 60} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"riptide"), (enchantmentID)enchantmentId, 3, { {17, 50}, {24, 50}, {31, 50} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"sharpness"), (enchantmentID)enchantmentId, 5, { {1, 21}, {12, 32}, {23, 43}, {34, 54}, {45, 65} }, 10);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"silk_touch"), (enchantmentID)enchantmentId, 1, { {15, 61} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"smite"), (enchantmentID)enchantmentId, 5, { {5, 25}, {13, 33}, {21, 41}, {29, 49}, {37, 57} }, 5);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"soul_speed"), (enchantmentID)enchantmentId, 3, { {10, 25}, {20, 35}, {30, 45} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"sweeping_edge"), (enchantmentID)enchantmentId, 3, { {5, 20}, {14, 29}, {23, 38} }, 2);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"thorns"), (enchantmentID)enchantmentId, 3, { {10, 61}, {30, 71}, {50, 81} }, 1);
	enchantmentId++;
	enchantmentDataList[enchantmentId] = new enchantmentData(std::wstring(L"unbreaking"), (enchantmentID)enchantmentId, 3, { {5, 61}, {13, 71}, {21, 81} }, 5);
	enchantmentId++;

	// enchantmentDataList.update();

	noteDataList = idList<noteData*, noteTypeID>(
		{
			new noteData(L"banjo"),
			new noteData(L"bass"),
			new noteData(L"bassattack"),
			new noteData(L"bd"),
			new noteData(L"bell"),
			new noteData(L"bit"),
			new noteData(L"cow_bell"),
			new noteData(L"didgeridoo"),
			new noteData(L"flute"),
			new noteData(L"guitar"),
			new noteData(L"harp"),
			new noteData(L"harp2"),
			new noteData(L"hat"),
			new noteData(L"icechime"),
			new noteData(L"iron_xylophone"),
			new noteData(L"pling"),
			new noteData(L"snare"),
			new noteData(L"xylobone"),
		});

	mushroomColorDataList = idList<mushroomColorData*, mushroomColorID>(
		{ new mushroomColorData(L"red"),
		 new mushroomColorData(L"brown") });

	netherVineTypeDataList = idList<netherVineTypeData*, netherVineTypeID>(
		{ new netherVineTypeData(L"weeping"),
		 new netherVineTypeData(L"twisting") });

	woodTypeDataList = idList<woodTypeData*, woodTypeID>(
		{ new woodTypeData(L"oak"),
		 new woodTypeData(L"spruce"),
		 new woodTypeData(L"birch"),
		 new woodTypeData(L"jungle"),
		 new woodTypeData(L"acacia"),
		 new woodTypeData(L"dark_oak"),
		 new woodTypeData(L"crimson"),
		 new woodTypeData(L"warped") });

	fireworkShapeDataList = idList<fireworkShapeData*, fireworkShapeID>(
		{ new fireworkShapeData(L"small_ball"),
		 new fireworkShapeData(L"large_ball"),
		 new fireworkShapeData(L"star_shaped"),
		 new fireworkShapeData(L"creeper_shaped"),
		 new fireworkShapeData(L"burst") });

	particleTypeDataList = idList<particleTypeData*, particleID>({
		new particleTypeData(L"ambient_entity_effect"),
		new particleTypeData(L"angry_villager"),
		new particleTypeData(L"ash"),
		new particleTypeData(L"barrier"),
		new particleTypeData(L"block"),
		new particleTypeData(L"bubble"),
		new particleTypeData(L"bubble_pop"),
		new particleTypeData(L"bubble_column_up"),
		new particleTypeData(L"campfire_cosy_smoke"),
		new particleTypeData(L"campfire_signal_smoke"),
		new particleTypeData(L"cloud"),
		new particleTypeData(L"composter"),
		new particleTypeData(L"crimson_spore"),
		new particleTypeData(L"crit"),
		new particleTypeData(L"current_down"),
		new particleTypeData(L"damage_indicator"),
		new particleTypeData(L"dolphin"),
		new particleTypeData(L"dragon_breath"),
		new particleTypeData(L"dripping_lava"),
		new particleTypeData(L"dripping_obsidian_tear"),
		new particleTypeData(L"dripping_water"),
		new particleTypeData(L"dust"),
		new particleTypeData(L"dust_color_transition"),
		new particleTypeData(L"effect"),
		new particleTypeData(L"elder_guardian"),
		new particleTypeData(L"enchant"),
		new particleTypeData(L"enchanted_hit"),
		new particleTypeData(L"end_rod"),
		new particleTypeData(L"entity_effect"),
		new particleTypeData(L"explosion_emitter"),
		new particleTypeData(L"explosion"),
		new particleTypeData(L"falling_dust"),
		new particleTypeData(L"falling_lava"),
		new particleTypeData(L"falling_obsidian_tear"),
		new particleTypeData(L"falling_water"),
		new particleTypeData(L"firework"),
		new particleTypeData(L"fishing"),
		new particleTypeData(L"flame"),
		new particleTypeData(L"flash"),
		new particleTypeData(L"happy_villager"),
		new particleTypeData(L"heart"),
		new particleTypeData(L"instant_effect"),
		new particleTypeData(L"item"),
		new particleTypeData(L"item_slime"),
		new particleTypeData(L"item_snowball"),
		new particleTypeData(L"landing_lava"),
		new particleTypeData(L"landing_obsidian_tear"),
		new particleTypeData(L"large_smoke"),
		new particleTypeData(L"lava"),
		new particleTypeData(L"mycelium"),
		new particleTypeData(L"nautilus"),
		new particleTypeData(L"note"),
		new particleTypeData(L"poof"),
		new particleTypeData(L"portal"),
		new particleTypeData(L"rain"),
		new particleTypeData(L"smoke"),
		new particleTypeData(L"sneeze"),
		new particleTypeData(L"soul"),
		new particleTypeData(L"soul_fire_flame"),
		new particleTypeData(L"spit"),
		new particleTypeData(L"splash"),
		new particleTypeData(L"squid_ink"),
		new particleTypeData(L"sweep_attack"),
		new particleTypeData(L"totem_of_undying"),
		new particleTypeData(L"underwater"),
		new particleTypeData(L"vibration"),
		new particleTypeData(L"warped_spore"),
		new particleTypeData(L"witch") });

	structureDataList = idList<structureData*, structureID>(
		{
			new structureData(L"mine_shaft"),
			new structureData(L"stronghold"),
			new structureData(L"fossil"),
			new structureData(L"buried_treasure"),
			new structureData(L"ruined_portal"),
			new structureData(L"ocean_ruins"),
			new structureData(L"shipwreck"),
			new structureData(L"ocean_monument"),
			new structureData(L"desert_pyramid"),
			new structureData(L"igloo"),
			new structureData(L"jungle_pyramid"),
			new structureData(L"pillager_outpost"),
			new structureData(L"swamp_hut"),
			new structureData(L"village"),
			new structureData(L"woodland_mansion"),
			new structureData(L"nether_fortress"),
			new structureData(L"bastion_remnant"),
			new structureData(L"nether_fossil"),
			new structureData(L"end_city"),
			new structureData(L"dungeon"),
			new structureData(L"hot_air_balloon"),
			new structureData(L"wind_mill"),
			new structureData(L"floating_ship"),
		});

	constexpr color hotBiomeColor = hexToColor(0xbfb755);
	constexpr color oceanBiomeColor = hexToColor(0x8eb971);
	int currentBiomeID = 0;
	biomeDataList = idList<biomeData*, biomeID>(fastList<biomeData*>((int)biomeID::biomeCount));
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"ocean"), hexToColor(0x0000FF), oceanBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"swamp"), hexToColor(0x808000), hexToColor(0x6a7039));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"plains"), hexToColor(0x00ff00), hexToColor(0x91bd59));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"savanna"), hexToColor(0xff8080), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"desert"), hexToColor(0xffff00), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"taiga"), hexToColor(0x80ff00), hexToColor(0x86b783));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"forest"), hexToColor(0x404000), hexToColor(0x79c05a));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"dark_forest"), hexToColor(0x004000), hexToColor(0x507A32));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"jungle"), hexToColor(0x406000), hexToColor(0x59C93C));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"snowy_tundra"), hexToColor(0xffffff), hexToColor(0x80B497));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"ice_spikes"), hexToColor(0x40ffff), hexToColor(0xff4000));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"mushroom_fields"), hexToColor(0x802020), hexToColor(0x55C93F));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"mountains"), hexToColor(0x808080), hexToColor(0x8AB689));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"death_valley"), hexToColor(0x804000), hexToColor(0x8AB689));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"badlands"), hexToColor(0xff8000), hexToColor(0x90814D));
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"nether_wastes"), hexToColor(0xff0000), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"crimson_forest"), hexToColor(0x402020), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"warped_forest"), hexToColor(0x00ffff), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"soulsand_valley"), hexToColor(0x804080), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"basalt_deltas"), hexToColor(0x404040), hotBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"the_end"), hexToColor(0xffff80), oceanBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"end_void"), hexToColor(0xc0c0c0), oceanBiomeColor);
	currentBiomeID++;
	biomeDataList[currentBiomeID] = new biomeData(std::wstring(L"end_lands"), hexToColor(0xc040ff), oceanBiomeColor);
	currentBiomeID++;

	int currentStatusEffectID = 0;

	statusEffectDataList = idList<statusEffectData*, statusEffectID>(fastList<statusEffectData*>((int)statusEffectID::count));

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"luck"), (statusEffectID)currentStatusEffectID, hexToColor(0x339900));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"unluck"), (statusEffectID)currentStatusEffectID, hexToColor(0xC0A44D));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"saturation"), (statusEffectID)currentStatusEffectID, hexToColor(0xF82423));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"hunger"), (statusEffectID)currentStatusEffectID, hexToColor(0x587653));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"speed"), (statusEffectID)currentStatusEffectID, hexToColor(0x7CAFC6));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"slowness"), (statusEffectID)currentStatusEffectID, hexToColor(0x5A6C81));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"haste"), (statusEffectID)currentStatusEffectID, hexToColor(0xD9C043));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"mining_fatigue"), (statusEffectID)currentStatusEffectID, hexToColor(0x4A4217));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"strength"), (statusEffectID)currentStatusEffectID, hexToColor(0x932423));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"weakness"), (statusEffectID)currentStatusEffectID, hexToColor(0x484D48));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"regeneration"), (statusEffectID)currentStatusEffectID, hexToColor(0xCD5CAB));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"poison"), (statusEffectID)currentStatusEffectID, hexToColor(0x4E9331));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"levitation"), (statusEffectID)currentStatusEffectID, hexToColor(0xCEFFFF));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"slow_falling"), (statusEffectID)currentStatusEffectID, hexToColor(0xCEFFFF));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"night_vision"), (statusEffectID)currentStatusEffectID, hexToColor(0x1F1FA1));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"invisibility"), (statusEffectID)currentStatusEffectID, hexToColor(0x7F8392));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"instant_health"), (statusEffectID)currentStatusEffectID, hexToColor(0xF82423));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"instant_damage"), (statusEffectID)currentStatusEffectID, hexToColor(0x430A09));
	currentStatusEffectID++;

	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"jump_boost"), (statusEffectID)currentStatusEffectID, hexToColor(0x786297));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"nausea"), (statusEffectID)currentStatusEffectID, hexToColor(0x551D4A));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"resistance"), (statusEffectID)currentStatusEffectID, hexToColor(0x99453A));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"fire_resistance"), (statusEffectID)currentStatusEffectID, hexToColor(0xE49A3A));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"water_breathing"), (statusEffectID)currentStatusEffectID, hexToColor(0x2E5299));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"blindness"), (statusEffectID)currentStatusEffectID, hexToColor(0x1F1F23));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"wither"), (statusEffectID)currentStatusEffectID, hexToColor(0x352A27));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"health_boost"), (statusEffectID)currentStatusEffectID, hexToColor(0xF87D23));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"absorption"), (statusEffectID)currentStatusEffectID, hexToColor(0x2552A5));
	currentStatusEffectID++;
	statusEffectDataList[currentStatusEffectID] = new statusEffectData(std::wstring(L"glowing"), (statusEffectID)currentStatusEffectID, hexToColor(0x94A061));
	currentStatusEffectID++;

	gameModeDataList = idList<gameModeData*, gameModeID>(fastList<gameModeData*>((int)gameModeID::gameModesCount));

	int currentGameModeID = 0;
	gameModeDataList[currentGameModeID] = new gameModeData(std::wstring(L"survival"), false, true);
	currentGameModeID++;
	gameModeDataList[currentGameModeID] = new gameModeData(std::wstring(L"creative"), true, false);
	currentGameModeID++;
	gameModeDataList[currentGameModeID] = new gameModeData(std::wstring(L"adventure"), false, true);
	currentGameModeID++;
	gameModeDataList[currentGameModeID] = new gameModeData(std::wstring(L"spectator"), true, false);
	currentGameModeID++;
}

static void setFoodValues()
{
	// add food values
	itemList[itemID::dried_kelp]->setEatingValues(1, 0.6);
	itemList[itemID::carrot]->setEatingValues(3, 3.6);

	itemList[itemID::potato]->setEatingValues(1, 0.6);
	itemList[itemID::bread]->setEatingValues(5, 6.0);

	itemList[(itemID)blockID::beetroots]->setEatingValues(1, 1.2);
	itemList[itemID::beetroot_soup]->setEatingValues(6, 7.2);

	itemList[itemID::baked_potato]->setEatingValues(5, 6.0);
	itemList[itemID::golden_carrot]->setEatingValues(6, 14.4);

	itemList[itemID::rotten_flesh]->setEatingValues(4, 0.8);

	itemList[itemID::beef]->setEatingValues(3, 1.8);
	itemList[itemID::cooked_beef]->setEatingValues(8, 12.8);

	itemList[itemID::mutton]->setEatingValues(2, 1.2);
	itemList[itemID::cooked_mutton]->setEatingValues(6, 9.6);

	itemList[itemID::porkchop]->setEatingValues(3, 1.8);
	itemList[itemID::cooked_porkchop]->setEatingValues(8, 12.8);

	itemList[itemID::chicken]->setEatingValues(2, 1.2);
	itemList[itemID::cooked_chicken]->setEatingValues(6, 7.2);

	itemList[itemID::rabbit]->setEatingValues(3, 1.8);
	itemList[itemID::cooked_rabbit]->setEatingValues(5, 6);

	itemList[itemID::cod]->setEatingValues(2, 0.4);
	itemList[itemID::cooked_cod]->setEatingValues(5, 6);

	itemList[itemID::salmon]->setEatingValues(2, 0.2);
	itemList[itemID::cooked_salmon]->setEatingValues(6, 9.6);

	itemList[itemID::tropical_fish]->setEatingValues(1, 0.2);

	itemList[itemID::pufferfish]->setEatingValues(1, 0.2);

	itemList[itemID::spider_eye]->setEatingValues(2, 3.2);

	itemList[itemID::apple]->setEatingValues(4, 2.4);
	itemList[itemID::golden_apple]->setEatingValues(4, 9.6);
	itemList[itemID::enchanted_golden_apple]->setEatingValues(4, 9.6);
	itemList[itemID::mushroom_stew]->setEatingValues(6, 7.2);

	itemList[itemID::chorus_fruit]->setEatingValues(4, 2.4);
	itemList[itemID::melon_slice]->setEatingValues(2, 1.2);

	itemList[itemID::poisonous_potato]->setEatingValues(1, 0.6);
}

static void addFuelProperties() {
	// add fuel properties
	itemList[(int)blockID::crafting_table]->fuelTicks = 300;
	itemList[(int)itemID::coal]->fuelTicks = 1600;
	itemList[(int)blockID::torch]->fuelTicks = 1700 / 4; // coal + stick = 4 torches
	itemList[(int)blockID::ladder]->fuelTicks = 300;
	itemList[(int)itemID::lava_bucket]->fuelTicks = 20000;
	itemList[(int)blockID::note_block]->fuelTicks = 300;
	itemList[(int)blockID::jukebox]->fuelTicks = 300;
	itemList[(int)itemID::stick]->fuelTicks = 100;
	itemList[(int)itemID::charcoal]->fuelTicks = 1600;
	itemList[(int)itemID::bowl]->fuelTicks = 100;

	for (int i = 0; i < woodTypeCount; i++)
	{
		if (i < normalTreeTypeCount) {
			itemList[(int)getTreeBlock((woodTypeID)i, treeItemTypeID::log)]->fuelTicks = 300;
			itemList[(int)getTreeBlock((woodTypeID)i, treeItemTypeID::strippedLog)]->fuelTicks = 300;
			itemList[(int)getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->fuelTicks = 300;
			itemList[(int)getTreeBlock((woodTypeID)i, treeItemTypeID::leaves)]->fuelTicks = 25;
			itemList[(int)blockID::wood_stairs + i]->fuelTicks = 300;
			itemList[(int)blockID::wood_fence + i]->fuelTicks = 300;
			itemList[(int)blockID::wood_fence_gate + i]->fuelTicks = 300;
			itemList[(int)blockID::wood_trapdoor + i]->fuelTicks = 300;
			itemList[(int)blockID::wood_door + i]->fuelTicks = 300;
			itemList[(int)blockID::wood_slab + i]->fuelTicks = 150;
			itemList[(int)blockID::wood_button + i]->fuelTicks = 150;
			itemList[(int)blockID::wood_sapling + i]->fuelTicks = 100;
			itemList[(int)blockID::wood_door + i]->fuelTicks = 200;
			itemList[(int)blockID::wood_boat + i]->fuelTicks = 1200;
		}
		itemList[(int)blockID::wood_door + i]->maxStackSize = 0x10;
	}
	for (int toolTierIndex = 0; toolTierIndex < toolTierCount; toolTierIndex++) {
		for (int toolTypeIndex = 0; toolTypeIndex < materialToolTypeCount; toolTypeIndex++) {
			itemList[getMaterialTool((toolTypeID)toolTypeIndex, (harvestTierID)toolTierIndex)]->fuelTicks = toolTierIndex == harvestTierID::woodHarvestTier ? 200 : 50;
		}
	}
}

static void loadMusic()
{
	mainMenuBackgroundMusic = std::make_shared<audioCollection>(menuMusicFolder / L"menu", music);
	overWorldBackgroundMusic = std::make_shared<audioCollection>(gameMusicFolder / L"calm", music);
	overWorldBackgroundMusic->addAudioFileName(gameMusicFolder / L"hal");
	overWorldBackgroundMusic->addAudioFileName(gameMusicFolder / L"hal");
	overWorldBackgroundMusic->addAudioFileName(gameMusicFolder / L"nuance");
	overWorldBackgroundMusic->addAudioFileName(gameMusicFolder / L"piano");
	netherMusic = std::make_shared<audioCollection>(netherMusicFolder / L"nether", music);
	endMusic = std::make_shared<audioCollection>(endMusicFolder / L"end", music);
	bossMusic = std::make_shared<audioCollection>(endMusicFolder / L"boss", music);
	creditsMusic = std::make_shared<audioCollection>(endMusicFolder / L"credits", music);
	creativeModeMusic = std::make_shared<audioCollection>(creativeModeMusicFolder / L"creative", music);
	crimsonForestMusic = std::make_shared<audioCollection>(netherMusicFolder / L"crimson_forest" / L"chrysopoeia", music);
	netherWastesMusic = std::make_shared<audioCollection>(netherMusicFolder / L"nether_wastes" / L"rubedo", music);
	soulSandValleyMusic = std::make_shared<audioCollection>(netherMusicFolder / L"soulsand_valley" / L"so_below", music);
	crimsonForestMusic->addCollection(*netherMusic);
	netherWastesMusic->addCollection(*netherMusic);
	soulSandValleyMusic->addCollection(*netherMusic);

	for (size_t i = 0; i < musicDiscTypeCount; i++)
	{
		recordsMusic[i] = std::make_shared<audioCollection>(recordsMusicFolder / (musicDiscNames[i]), music);
	}
	waterMusic = std::make_shared<audioCollection>(waterMusicFolder / L"axolotl", music);
	waterMusic->addAudioFileName(waterMusicFolder / L"dragon_fish");
	waterMusic->addAudioFileName(waterMusicFolder / L"shuniji");
}
static void loadTags()
{
	tagList = fastList<tag*>();
	for (const auto& currentFolder : getResourceLocations(mainTagFolder))
	{
		// load tags
		for (const auto& folderIterator : stdFileSystem::directory_iterator(currentFolder))
		{
			const stdPath tagFolder = folderIterator.path();
			for (const auto& fileIterator : stdFileSystem::directory_iterator(tagFolder))
			{
				const std::wstring& fileNameWithoutExtension = fileIterator.path().stem().wstring();
				const std::wstring& extension = fileIterator.path().extension().wstring();
				if (extension == jsonFileExtension)
				{
					if (getTagListIndexByName(fileNameWithoutExtension) == std::wstring::npos)
					{
						// not a subtag of previously added tags
						readTag(fileNameWithoutExtension, tagFolder);
					}
				}
			}
		}
	}

	for (size_t i = 0; i < tagList.size; i++)
	{
		// count comparables
		if (tagList[i]->taggedComparables->size == 0)
		{

			delete tagList[i];
			tagList.erase(i);
		}
	}

	tagList.update();
}

static void loadLootTables()
{
	// load loot tables
	// chest loot
	// we don'T know what chest loot tables we're going to use, as they can be defined in a chest as nbtdatavalue
	// for each chest loot table, we need to only read the 'highest' one
	// so we iterate from highest to lowest and read all tables which aren'T in the map yet
	for (const auto& currentFolder : getResourceLocations(chestLootTablesFolder) | std::views::reverse)
	{
		for (const auto& fileIterator : stdFileSystem::directory_iterator(currentFolder))
		{
			const stdPath& path = fileIterator.path().wstring();
			const std::wstring& fileNameWithoutExtension = fileIterator.path().stem().wstring();
			const std::wstring& extension = fileIterator.path().extension().wstring();
			if (extension == jsonFileExtension)
			{
				if (!chestLootTables.contains(fileNameWithoutExtension))
				{
					std::shared_ptr<lootTable> table = readLootTable(path);
					chestLootTables.insert(std::pair<std::wstring, std::shared_ptr<lootTable>>(fileNameWithoutExtension, table));
				}
			}
		}
	}

	// block loot
	// iterate over blocks instead of files, because we can assume that there are more block loot tables than blocks
	for (block* const& b : blockList)
	{
		stdPath location;
		if (getLastResourceLocation(blockLootTablesFolder / (b->name + L".json"), location))
		{
			// loot table found for this block
			b->dropsWhenHarvested = readLootTable(location);
		}
	}
	// entity loot
	// same story here
	for (entityData* const& e : entityDataList)
	{
		if (isMob(e->identifier))
		{
			stdPath location;
			if (getLastResourceLocation(entityLootTablesFolder / (e->name + L".json"), location))
			{
				// loot table found for this block
				((mobData*)e)->dropsWhenKilled = readLootTable(location);
			}
		}
	}

	// block loot
	// for (const auto &fileIterator : stdFileSystem::directory_iterator(blockLootTablesFolder))
	//{
	//	const stdPath &path = fileIterator.path().wstring();
	//	const std::wstring &fileNameWithoutExtension = fileIterator.path().stem().wstring();
	//	const std::wstring &extension = fileIterator.path().extension().wstring();
	//	if (extension == jsonFileExtension)
	//	{
	//		const blockID &blockListIndex = blockList.getIDByName(fileNameWithoutExtension);
	//		if ((int)blockListIndex != -1)
	//		{
	//			blockList[blockListIndex]->dropsWhenHarvested = readLootTable(path);
	//		}
	//	}
	//}
	// mob loot
	// for (const auto &fileIterator : stdFileSystem::directory_iterator(entityLootTablesFolder))
	//{
	//	const stdPath &path = fileIterator.path().wstring();
	//	const std::wstring &fileNameWithoutExtension = fileIterator.path().stem().wstring();
	//	const std::wstring &extension = fileIterator.path().extension().wstring();
	//	if (extension == jsonFileExtension)
	//	{
	//		cint &entityListIndex = getEntityIDByName(fileNameWithoutExtension);
	//		if (entityListIndex != -1)
	//		{
	//			((mobData *)entityDataList[entityListIndex])->dropsWhenKilled = readLootTable(path);
	//		}
	//	}
	//}
}
static void loadRecipes()
{
	craftingRecipes = std::vector<recipe*>();
	furnaceRecipes = std::vector<furnaceRecipe*>();
	for (const auto& currentFolder : getResourceLocations(recipeFolder) | std::views::reverse)
		for (const auto& fileIterator : stdFileSystem::directory_iterator(currentFolder))
		{
			std::wstring path = fileIterator.path().wstring();
			const jsonContainer& content = readJson(stringToWString(readAllText(path)));
			// this might cause double recipes, but the recipes which matter the most will be on top, so it'll match correctly
			// todo: implement algorithm that eliminates duplicate recipes to save memory
			readRecipe(content);
		}
}
static void loadStructures()
{
	structureList = std::vector<structure*>();
	for (const auto& currentFolder : getResourceLocations(structureFolder) | std::views::reverse)
		for (const auto& fileIterator : stdFileSystem::recursive_directory_iterator(currentFolder))
		{
			const stdPath& path = fileIterator.path().wstring();
			const std::wstring& extension = fileIterator.path().extension().wstring();
			if (extension == nbtFileExtension)
			{
				const stdPath pathWithoutExtension = stdFileSystem::relative(path, currentFolder).replace_extension();
				if (!getStructureByPath(pathWithoutExtension))
				{
					// this structure isn'T replaced by some structure of a resourcepack higher in order
					structure* s = new structure(pathWithoutExtension);
					s->serialize(path, false);
					structureList.push_back(s);
				}
			}
		}
}

static void loadBlockPowerProperties()
{
	blockList[blockID::redstone_block]->emittanceLevel[2] = maxPowerLevel;

	blockList[blockID::redstone_wire]->filterStrength[2] = 1;

	// for blocks that use power to do something
	cint& deviceFilterStrength = maxPowerLevel / 0x20;

	blockList[blockID::powered_rail]->filterStrength[2] = maxPowerLevel / 0x40;
	blockList[blockID::redstone_lamp]->filterStrength[2] = deviceFilterStrength;
	blockList[blockID::dispenser]->filterStrength[2] = deviceFilterStrength;
	blockList[blockID::dropper]->filterStrength[2] = deviceFilterStrength;
	blockList[blockID::iron_door]->filterStrength[2] = deviceFilterStrength;

	blockList[blockID::note_block]->filterStrength[2] = deviceFilterStrength;
	blockList[blockID::jukebox]->filterStrength[2] = deviceFilterStrength;

	blockList[blockID::gold_block]->filterStrength[2] = maxPowerLevel / 0x100;
	blockList[blockID::iron_block]->filterStrength[2] = maxPowerLevel / 0x80;
	blockList[blockID::quartz_block]->filterStrength[2] = maxPowerLevel / 0x40;
	blockList[blockID::glowstone]->filterStrength[2] = maxPowerLevel / 0x20;
	blockList[blockID::water]->filterStrength[2] = maxPowerLevel / 0x10;
	blockList[blockID::dirt]->filterStrength[2] = maxPowerLevel / 0x2;
	blockList[blockID::farmland]->filterStrength[2] = maxPowerLevel / 0x2;
	blockList[blockID::grass_block]->filterStrength[2] = maxPowerLevel / 0x4;
	blockList[blockID::coal_block]->filterStrength[2] = maxPowerLevel / 0x4;

	blockList[blockID::gold_ore]->filterStrength[2] = maxPowerLevel / 0x10;
	blockList[blockID::iron_ore]->filterStrength[2] = maxPowerLevel / 0x8;

	blockList[blockID::tnt]->filterStrength[2] = 1;

	for (int i = 0; i < (int)sandStoneTypeID::count * (int)sandStoneColorID::count; i++)
	{
		blockList[(int)blockID::sandstone + i]->filterStrength[2] = maxPowerLevel / 0x4;
	}
	blockList[blockID::sand]->filterStrength[2] = maxPowerLevel / 0x4;
	blockList[blockID::gravel]->filterStrength[2] = maxPowerLevel / 0x8;
}

static void loadBlocks()
{
	std::shared_ptr<audioCollection> digGrass = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"grass", AudioType::sound);
	std::shared_ptr<audioCollection> stepGrass = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"grass", AudioType::sound);
	std::shared_ptr<audioCollection> digStone = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"stone", AudioType::sound);
	std::shared_ptr<audioCollection> stepStone = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"stone", AudioType::sound);
	std::shared_ptr<audioCollection> digWood = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"wood", AudioType::sound);
	std::shared_ptr<audioCollection> stepWood = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"wood", AudioType::sound);

	std::shared_ptr<audioCollection> stepStem = std::make_shared<audioCollection>(blockSoundFolder / L"stem" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakStem = std::make_shared<audioCollection>(blockSoundFolder / L"stem" / L"break", AudioType::sound);

	std::shared_ptr<audioCollection> digGravel = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"gravel", AudioType::sound);
	std::shared_ptr<audioCollection> stepGravel = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"gravel", AudioType::sound);
	std::shared_ptr<audioCollection> digSand = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"sand", AudioType::sound);
	std::shared_ptr<audioCollection> stepSand = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"sand", AudioType::sound);
	std::shared_ptr<audioCollection> breakSoulSand = std::make_shared<audioCollection>(blockSoundFolder / L"soul_sand" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepSoulSand = std::make_shared<audioCollection>(blockSoundFolder / L"soul_sand" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakGlass = std::make_shared<audioCollection>(generalSoundFolder / L"random" / L"glass", AudioType::sound);
	std::shared_ptr<audioCollection> stepCloth = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"cloth", AudioType::sound);
	std::shared_ptr<audioCollection> breakCrop = std::make_shared<audioCollection>(generalSoundFolder / L"item" / L"plant" / L"crop", AudioType::sound);
	std::shared_ptr<audioCollection> stepLadder = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"ladder", AudioType::sound);
	std::shared_ptr<audioCollection> stepSnow = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"snow", AudioType::sound);
	std::shared_ptr<audioCollection> placeLantern = std::make_shared<audioCollection>(blockSoundFolder / L"lantern" / L"place", AudioType::sound);
	std::shared_ptr<audioCollection> breakLantern = std::make_shared<audioCollection>(blockSoundFolder / L"lantern" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> digWetGrass = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"wet_grass", AudioType::sound);
	std::shared_ptr<audioCollection> stepWetGrass = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"wet_grass", AudioType::sound);
	std::shared_ptr<audioCollection> stepNetherrack = std::make_shared<audioCollection>(blockSoundFolder / L"netherrack" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> digNetherrack = std::make_shared<audioCollection>(blockSoundFolder / L"netherrack" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepNylium = std::make_shared<audioCollection>(blockSoundFolder / L"nylium" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> digNylium = std::make_shared<audioCollection>(blockSoundFolder / L"nylium" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepShroomLight = std::make_shared<audioCollection>(blockSoundFolder / L"shroomlight" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> digShroomLight = std::make_shared<audioCollection>(blockSoundFolder / L"shroomlight" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepNetherBrick = std::make_shared<audioCollection>(blockSoundFolder / L"nether_bricks" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> digNetherBrick = std::make_shared<audioCollection>(blockSoundFolder / L"nether_bricks" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> breakFungus = std::make_shared<audioCollection>(blockSoundFolder / L"fungus" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepNetherWart = std::make_shared<audioCollection>(blockSoundFolder / L"netherwart" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakNetherWart = std::make_shared<audioCollection>(blockSoundFolder / L"netherwart" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepBone = std::make_shared<audioCollection>(blockSoundFolder / L"bone_block" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> digBone = std::make_shared<audioCollection>(blockSoundFolder / L"bone_block" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> breakAncientDebris = std::make_shared<audioCollection>(blockSoundFolder / L"ancient_debris" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> placeBerryBush = std::make_shared<audioCollection>(blockSoundFolder / L"sweet_berry_bush" / L"place", AudioType::sound);
	std::shared_ptr<audioCollection> breakBerryBush = std::make_shared<audioCollection>(blockSoundFolder / L"sweet_berry_bush" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepHoney = std::make_shared<audioCollection>(blockSoundFolder / L"honeyblock" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakHoney = std::make_shared<audioCollection>(blockSoundFolder / L"honeyblock" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepScaffolding = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"scaffold", AudioType::sound);
	std::shared_ptr<audioCollection> placeScaffolding = std::make_shared<audioCollection>(blockSoundFolder / L"scaffold" / L"place", AudioType::sound);
	std::shared_ptr<audioCollection> stepChain = std::make_shared<audioCollection>(blockSoundFolder / L"chain" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakChain = std::make_shared<audioCollection>(blockSoundFolder / L"chain" / L"break", AudioType::sound);
	std::shared_ptr<audioCollection> stepCoral = std::make_shared<audioCollection>(generalSoundFolder / L"step" / L"coral", AudioType::sound);
	std::shared_ptr<audioCollection> breakCoral = std::make_shared<audioCollection>(generalSoundFolder / L"dig" / L"coral", AudioType::sound);
	std::shared_ptr<audioCollection> stepRoots = std::make_shared<audioCollection>(blockSoundFolder / L"roots" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakRoots = std::make_shared<audioCollection>(blockSoundFolder / L"roots" / L"break", AudioType::sound);

	std::shared_ptr<audioCollection> stepBamboo = std::make_shared<audioCollection>(blockSoundFolder / L"bamboo" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> placeBamboo = std::make_shared<audioCollection>(blockSoundFolder / L"bamboo" / L"place", AudioType::sound);
	std::shared_ptr<audioCollection> hitBambooSapling = std::make_shared<audioCollection>(blockSoundFolder / L"bamboo" / L"sapling_hit", AudioType::sound);
	std::shared_ptr<audioCollection> placeBambooSapling = std::make_shared<audioCollection>(blockSoundFolder / L"bamboo" / L"sapling_place", AudioType::sound);

	std::shared_ptr<audioCollection> stepSoulSoil = std::make_shared<audioCollection>(blockSoundFolder / L"soul_soil" / L"step", AudioType::sound);
	std::shared_ptr<audioCollection> breakSoulSoil = std::make_shared<audioCollection>(blockSoundFolder / L"soul_soil" / L"break", AudioType::sound);

	for (int i = 0; i < (int)noteTypeID::count; i++)
	{
		noteSounds[i] = std::make_shared<audioCollection>(noteSoundFolder / noteDataList[i]->name, AudioType::sound);
	}

	popSound = std::make_shared<audioCollection>(generalSoundFolder / L"random" / L"pop", AudioType::sound);
	experienceSound = std::make_shared<audioCollection>(generalSoundFolder / L"random" / L"orb", AudioType::sound);
	levelUpSound = std::make_shared<audioCollection>(generalSoundFolder / L"random" / L"levelup", AudioType::sound);

	chestOpenSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"chestopen", AudioType::sound);
	chestCloseSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"chestclosed", AudioType::sound);

	enderChestOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"enderchest" / L"open", AudioType::sound);
	enderChestCloseSound = std::make_shared<audioCollection>(blockSoundFolder / L"enderchest" / L"close", AudioType::sound);

	barrelOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"barrel" / L"open", AudioType::sound);
	barrelCloseSound = std::make_shared<audioCollection>(blockSoundFolder / L"barrel" / L"close", AudioType::sound);

	stripLogSound = std::make_shared<audioCollection>(itemSoundFolder / L"axe" / L"strip", AudioType::sound);
	tillSound = std::make_shared<audioCollection>(itemSoundFolder / L"hoe" / L"till", AudioType::sound);
	eatSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"eat", AudioType::sound);
	burpSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"burp", AudioType::sound);
	waterBucketFillSound = std::make_shared<audioCollection>(itemSoundFolder / L"bucket" / L"fill", AudioType::sound);
	lavaBucketFillSound = std::make_shared<audioCollection>(itemSoundFolder / L"bucket" / L"fill_lava", AudioType::sound);
	waterBucketEmptySound = std::make_shared<audioCollection>(itemSoundFolder / L"bucket" / L"empty", AudioType::sound);
	lavaBucketEmptySound = std::make_shared<audioCollection>(itemSoundFolder / L"bucket" / L"empty_lava", AudioType::sound);
	milkSound = std::make_shared<audioCollection>(entitySoundFolder / L"cow" / L"milk", AudioType::sound);
	waterPaddleSound = std::make_shared<audioCollection>(entitySoundFolder / L"boat" / L"paddle_water", AudioType::sound);
	weakAttackSound = std::make_shared<audioCollection>(entitySoundFolder / L"player" / L"attack" / L"weak", AudioType::sound);
	strongAttackSound = std::make_shared<audioCollection>(entitySoundFolder / L"player" / L"attack" / L"strong", AudioType::sound);
	criticalAttackSound = std::make_shared<audioCollection>(entitySoundFolder / L"player" / L"attack" / L"crit", AudioType::sound);
	enchantSound = std::make_shared<audioCollection>(blockSoundFolder / L"enchantment_table" / L"enchant", AudioType::sound);
	equipSound = std::make_shared<audioCollection>(itemSoundFolder / L"armor" / L"equip_generic", AudioType::sound);

	woodenDoorOpenSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"door_open", AudioType::sound);
	woodenDoorCloseSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"door_close", AudioType::sound);

	ironDoorOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"iron_door" / L"open", AudioType::sound);
	ironDoorCloseSound = std::make_shared<audioCollection>(blockSoundFolder / L"iron_door" / L"close", AudioType::sound);

	fenceGateOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"fence_gate" / L"open", AudioType::sound);
	fenceGateCloseSound = std::make_shared<audioCollection>(blockSoundFolder / L"fence_gate" / L"close", AudioType::sound);

	woodenTrapDoorOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"wooden_trapdoor" / L"open", AudioType::sound);
	woodenTrapDoorCloseSound = std::make_shared<audioCollection>(blockSoundFolder / L"wooden_trapdoor" / L"close", AudioType::sound);

	ironTrapDoorOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"iron_trapdoor" / L"open", AudioType::sound);
	ironTrapDoorCloseSound = std::make_shared<audioCollection>(blockSoundFolder / L"iron_trapdoor" / L"close", AudioType::sound);

	flintAndSteelSound = std::make_shared<audioCollection>(generalSoundFolder / L"fire" / L"ignite", AudioType::sound);
	extinguishSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"fizz", AudioType::sound);
	sheepShearingSound = std::make_shared<audioCollection>(mobSoundFolder / L"sheep" / L"shear", AudioType::sound);
	burningDamageSound = std::make_shared<audioCollection>(entitySoundFolder / L"player" / L"hurt" / L"fire_hurt", AudioType::sound);
	portalDepartSound = std::make_shared<audioCollection>(generalSoundFolder / L"portal" / L"trigger", AudioType::sound);
	portalArriveSound = std::make_shared<audioCollection>(generalSoundFolder / L"portal" / L"travel", AudioType::sound);
	teleportFromSound = std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"portal", AudioType::sound);
	teleportToSound = std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"portal2", AudioType::sound);
	shootSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"bow", AudioType::sound);
	eyeOfEnderLaunchSound = std::make_shared<audioCollection>(entitySoundFolder / L"endereye" / L"endereye_launch", AudioType::sound);
	eyeOfEnderDropSound = std::make_shared<audioCollection>(entitySoundFolder / L"endereye" / L"dead", AudioType::sound);
	eyeOfEnderPlaceSound = std::make_shared<audioCollection>(blockSoundFolder / L"end_portal" / L"eyeplace", AudioType::sound);
	endPortalOpenSound = std::make_shared<audioCollection>(blockSoundFolder / L"end_portal" / L"endportal", AudioType::sound);
	fuseSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"fuse", AudioType::sound);
	explosionSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"explode", AudioType::sound);

	// ender dragon
	enderDragonWingsSound = std::make_shared<audioCollection>(mobSoundFolder / L"enderdragon" / L"wings", AudioType::sound);
	enderDragonDeathSound = std::make_shared<audioCollection>(mobSoundFolder / L"enderdragon" / L"end", AudioType::sound);

	// ghast
	fireBallShootSound = std::make_shared<audioCollection>(mobSoundFolder / L"ghast" / L"fireball4", AudioType::sound);
	ghastWarningSound = std::make_shared<audioCollection>(mobSoundFolder / L"ghast" / L"charge", AudioType::sound);

	// enderman
	endermanStareSound = std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"stare", AudioType::sound);
	endermanScreamSound = std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"stare", AudioType::sound);

	//villager
	villagerNoSound = std::make_shared<audioCollection>(mobSoundFolder / L"villager" / L"no", AudioType::sound);
	villagerYesSound = std::make_shared<audioCollection>(mobSoundFolder / L"villager" / L"yes", AudioType::sound);
	villagerHaggleSound = std::make_shared<audioCollection>(mobSoundFolder / L"villager" / L"haggle", AudioType::sound);

	fireSound = std::make_shared<audioCollection>(generalSoundFolder / L"fire" / L"fire", AudioType::sound);
	brewingFinishSound = std::make_shared<audioCollection>(blockSoundFolder / L"brewing_stand" / L"brew", AudioType::sound);
	bowHitSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"bowhit", AudioType::sound);
	clickSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"click", AudioType::sound);
	pistonExtendSound = std::make_shared<audioCollection>(tileSoundFolder / L"piston" / L"out", AudioType::sound);
	pistonRetractSound = std::make_shared<audioCollection>(tileSoundFolder / L"piston" / L"in", AudioType::sound);
	pumpkinCarveSound = std::make_shared<audioCollection>(blockSoundFolder / L"pumpkin" / L"carve", AudioType::sound);
	toolBreakSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"break", AudioType::sound);
	smithingTableUseSound = std::make_shared<audioCollection>(blockSoundFolder / L"smithing_table" / L"smithing_table", AudioType::sound);
	anvilUseSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"anvil_use", AudioType::sound);
	anvilLandSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"anvil_land", AudioType::sound);
	anvilBreakSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"anvil_break", AudioType::sound);

	smallSlimeSound = std::make_shared<audioCollection>(mobSoundFolder / L"slime" / L"small", AudioType::sound);
	bigSlimeSound = std::make_shared<audioCollection>(mobSoundFolder / L"slime" / L"big", AudioType::sound);
	slimeAttackSound = std::make_shared<audioCollection>(mobSoundFolder / L"slime" / L"attack", AudioType::sound);

	windSound = std::make_shared<audioCollection>(weatherSoundFolder / L"wind", AudioType::sound);

	drinkingSound = std::make_shared<audioCollection>(miscellaneousSoundFolder / L"drink", AudioType::sound);
	honeyDrinkingSound = std::make_shared<audioCollection>(itemSoundFolder / L"bottle" / L"drink_honey", AudioType::sound);
	bottleFillSound = std::make_shared<audioCollection>(itemSoundFolder / L"bottle" / L"fill", AudioType::sound);
	bottleEmptySound = std::make_shared<audioCollection>(itemSoundFolder / L"bottle" / L"empty", AudioType::sound);
	bottleFillDragonBreathSound = std::make_shared<audioCollection>(itemSoundFolder / L"bottle" / L"fill_dragonbreath", AudioType::sound);

	// load blocks
	blockList = idList<block*, blockID>(fastList<block*>(blockIDCount));

	// edit some textures

	// beware that we are now creating a texture, and not an texture!

	resolutionTexture* snowTexture = loadTextureFromResourcePack(blockTextureFolder / L"snow.png");
	resolutionTexture* farmlandTexture = loadTextureFromResourcePack(blockTextureFolder / L"farmland.png");
	cfp farmlandPart = 0.75; // the part of the farmland that is not 'rotated' to the player

	crectangle2 relativeDirtFarmlandRect = crectangle2(0, 0, 1, farmlandPart);

	fillTransformedBrushRectangle(getAbsoluteRect(dirtTexture->getClientRect(), relativeDirtFarmlandRect), getAbsoluteRect(crectangle2(farmlandTexture->getClientRect()), relativeDirtFarmlandRect), *dirtTexture, *farmlandTexture);

	resolutionTexture* unCroppedLanternTexture = loadTextureFromResourcePack(blockTextureFolder / L"lantern.png");
	resolutionTexture* lanternGraphics = new resolutionTexture(texture(cvect2<fsize_t>((fsize_t)(blockTextureSize * unCroppedLanternTexture->getScaleModifier()))), cvec2(blockTextureSize));
	crectangle2 lanternTextureRect = crectangle2(0, 39, 6, 9);
	cvec2 lanternDrawOffset = cvec2((blockTextureSize - lanternTextureRect.w) * 0.5, 0);
	fillTransformedBrushRectangle(lanternTextureRect, lanternDrawOffset, *unCroppedLanternTexture, *lanternGraphics);
	auto tallGrassTopTexture = loadTextureFromResourcePack(blockTextureFolder / L"tall_grass_top.png");

	auto largeFernTopTexture = loadTextureFromResourcePack(blockTextureFolder / L"large_fern_top.png");

	// auto brewingStandItemTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));
	// cbool hasBottle[brewingStandPotionCapacity]
	//{
	//	true,
	//	true,
	//	true,
	// };
	// renderBrewingStand(crectangle2(blockTextureRect), hasBottle, *brewingStandItemTexture);

	resolutionTexture* redStoneWireTopView = loadTextureFromResourcePack(blockTextureFolder / L"redstone_dust_line0.png");

	resolutionTexture* redStoneWireTexture = new resolutionTexture(texture(cvect2<fsize_t>((fsize_t)blockTextureSize)), cvec2(blockTextureSize));

	fillTransparentRectangle(crectangle2(blockTextureSize * (0.5 - (redstoneWireHeight * 0.5)), 0, blockTextureSize * redstoneWireHeight, blockTextureSize), 90, crectangle2(0, 0, blockTextureSize, blockTextureSize * redstoneWireHeight), *redStoneWireTopView, *redStoneWireTexture);

	int identifier = 0;

	constexpr int noLightFilter[(int)levelID::count]{
		0,
		0,
		maxPowerLevel };

	constexpr int lightBlocking[(int)levelID::count]{
		maxLightLevel,
		maxLightLevel,
		maxPowerLevel };

	constexpr int lightFiltering[(int)levelID::count]{
		maxLightLevel / 0x10,
		maxLightLevel / 0x10,
		maxPowerLevel };

	constexpr int doubleFiltering[(int)levelID::count]{
		maxLightLevel / 0x8,
		maxLightLevel / 0x8,
		maxPowerLevel };

	constexpr int sunlightPermeable[(int)levelID::count]{
		0x40,
		maxLightLevel,
		maxPowerLevel };

	constexpr int glowingLightSource[(int)levelID::count]{
		0,
		glowInTheDarkLightLevel,
		0 };

	constexpr int moodyLightSource[(int)levelID::count]{
		0,
		moodyLightLevel,
		0 };

	constexpr int brightLightSource[(int)levelID::count]{
		0,
		brightLightLevel,
		0 };

	blockList[identifier] = new block((blockID)identifier, -1, -1, airWeightPerCubicMeter, nullptr, std::wstring(L"air"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, airWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"structure_void.png"), std::wstring(L"structure_void"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, airWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"barrier.png"), std::wstring(L"barrier"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withHand }, noHarvestTier);
	identifier++;
	cint waterLightRange = 0x20;

	lightLevel waterDecreaseSpeed[(int)levelID::count] = { maxLightLevel / waterLightRange, maxLightLevel / waterLightRange, 0 };

	blockList[identifier] = new block((blockID)identifier, -1, -1, 1000, nullptr, std::wstring(L"water"), stepStone, stepStone, stepStone, digStone, digStone, waterDecreaseSpeed, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, 2500, nullptr, std::wstring(L"lava"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, true, brightLightSource);
	identifier++;

	// https://www.inchcalculator.com/lumber-weight-calculator/
	constexpr fp woodWeight[woodTypeCount]{
		750,  //"oak"),
		550,  //"spruce")
		670,  //"birch"),
		1200, //"jungle")
		1000, //"acacia")
		750,  //"dark_oak
		400,  //"crimson"
		400	  //"warped")
	};

	for (int i = 0; i < normalTreeTypeCount; i++)
	{
		// trees are at the background, so the light filters are turned down
		const std::wstring& logName = woodTypeDataList[i]->name + (i < normalTreeTypeCount ? std::wstring(L"_log") : std::wstring(L"_stem"));
		blockList[identifier] = new block((blockID)identifier, 2, 2, woodWeight[i], loadTextureFromResourcePack(blockTextureFolder / (logName + std::wstring(L".png"))), logName, stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 5, 5, true);
		identifier++;
		const std::wstring& strippedLogName = std::wstring(L"stripped_") + logName;
		blockList[identifier] = new block((blockID)identifier, 2, 2, woodWeight[i], loadTextureFromResourcePack(blockTextureFolder / (strippedLogName + std::wstring(L".png"))), strippedLogName, stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 5, 5, true);
		identifier++;
		const std::wstring& planksName = woodTypeDataList[i]->name + std::wstring(L"_planks");
		blockList[identifier] = new block((blockID)identifier, 2, 3, woodWeight[i], loadTextureFromResourcePack(blockTextureFolder / (planksName + std::wstring(L".png"))), planksName, stepWood, stepWood, stepWood, digWood, digWood, lightBlocking, { withAxe }, noHarvestTier, collisionTypeID::willCollide, 5, 20, true);
		identifier++;

		const std::wstring& leavesName = i == 6 ? std::wstring(L"nether_wart_block") : i == 7 ? std::wstring(L"warped_wart_block")
			: woodTypeDataList[i]->name + std::wstring(L"_leaves");

		blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (leavesName + std::wstring(L".png"))), leavesName, stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withSwordOrShears, withHoe }, noHarvestTier, collisionTypeID::willCollideTop, 30, 60, true);
		identifier++;
	}

	for (int i = normalTreeTypeCount; i < woodTypeCount; i++)
	{
		// trees are at the background, so the light filters are turned down
		const std::wstring& logName = woodTypeDataList[i]->name + std::wstring(L"_stem");
		blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (logName + std::wstring(L".png"))), logName, stepStem, stepStem, stepStem, breakStem, breakStem, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false);
		identifier++;
		const std::wstring& strippedLogName = std::wstring(L"stripped_") + logName;
		blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (strippedLogName + std::wstring(L".png"))), strippedLogName, stepStem, stepStem, stepStem, breakStem, breakStem, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false);
		identifier++;
		const std::wstring& planksName = woodTypeDataList[i]->name + std::wstring(L"_planks");
		blockList[identifier] = new block((blockID)identifier, 2, 3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (planksName + std::wstring(L".png"))), planksName, stepWood, stepWood, stepWood, digWood, digWood, lightBlocking, { withAxe }, noHarvestTier, collisionTypeID::willCollide);
		identifier++;

		std::wstring leavesName = i == 6 ? std::wstring(L"nether_wart_block") : i == 7 ? std::wstring(L"warped_wart_block")
			: woodTypeDataList[i]->name + std::wstring(L"_leaves");

		blockList[identifier] = new block((blockID)identifier, 1, 1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (leavesName + std::wstring(L".png"))), leavesName, stepNetherWart, stepNetherWart, stepNetherWart, breakNetherWart, breakNetherWart, lightFiltering, { withSwordOrShears }, noHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}

	constexpr harvestTierID oreBlockTiers[oreBlockTypeCount]{ woodHarvestTier, stoneHarvestTier, ironHarvestTier, stoneHarvestTier, ironHarvestTier, ironHarvestTier, ironHarvestTier, woodHarvestTier, woodHarvestTier };

	constexpr int minimalOreExperience[oreBlockTypeCount]{
		0,
		0,
		3,
		2,
		1,
		0,
		3,
		2,
		0,
	};
	constexpr int maximalOreExperience[oreBlockTypeCount]{
		2,
		0,
		7,
		5,
		5,
		0,
		7,
		5,
		1 };
	constexpr bool hasBlockForm[oreBlockTypeCount]{
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		false, // nether quartz ore does not have a block form
	};
	for (int i = 0; i < oreBlockTypeCount; i++)
	{
		if (hasBlockForm[i])
		{
			// mineral blocks
			const std::wstring blockName = (blockID)identifier == blockID::quartz_block ? std::wstring(L"quartz_block") : oreBlockNames[i] + std::wstring(L"_block");
			const std::wstring blockTextureName = (blockID)identifier == blockID::quartz_block ? std::wstring(L"quartz_block_side") : blockName;
			blockList[identifier] = new block((blockID)identifier, 5, 6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (blockTextureName + std::wstring(L".png"))), blockName, stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, oreBlockTiers[i]);
			identifier++;
		}
	}
	for (int i = 0; i < oreBlockTypeCount; i++)
	{
		// int minimumDrop = 1, maximumDrop = 1;
		std::wstring oreName = oreBlockNames[i] + std::wstring(L"_ore");
		// ores
		// const itemID oreDrop = (itemID)(smeltable[i] ? (int)oreBlockList[i] : ((int)itemID::coal + i));

		std::shared_ptr<audioCollection> oreStepSound = nullptr;
		std::shared_ptr<audioCollection> oreDigSound = nullptr;
		if ((blockID)identifier == blockID::nether_gold_ore || (blockID)identifier == blockID::nether_quartz_ore)
		{
			oreStepSound = stepNetherrack;
			oreDigSound = digNetherrack;
		}
		else
		{
			oreStepSound = stepStone;
			oreDigSound = digStone;
		}

		// if ((blockID)identifier == blockID::redstone_ore)
		//{
		//	minimumDrop = 4;
		//	maximumDrop = 5;
		// }
		// else if ((blockID)identifier == blockID::lapis_ore)
		//{
		//	minimumDrop = 4;
		//	maximumDrop = 9;
		// }
		// else if ((blockID)identifier == blockID::nether_gold_ore)
		//{
		//	minimumDrop = 2;
		//	maximumDrop = 6;
		// }

		blockList[identifier] = new block((blockID)identifier, 3, 3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (oreName + std::wstring(L".png"))), oreName, oreStepSound, oreStepSound, oreStepSound, oreDigSound, oreDigSound, lightBlocking, { withPickaxe }, oreBlockTiers[i], collisionTypeID::willCollide, 0, 0, false, false, noEmittance, experienceDrop(minimalOreExperience[i], maximalOreExperience[i]));
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 1.5, 6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"stone.png"), std::wstring(L"stone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cobblestone.png"), std::wstring(L"cobblestone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"smooth_stone.png"), std::wstring(L"smooth_stone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"stone_bricks.png"), std::wstring(L"stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"chiseled_stone_bricks.png"), std::wstring(L"chiseled_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 3, 3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"end_stone.png"), std::wstring(L"end_stone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3, 3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"end_stone_bricks.png"), std::wstring(L"end_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 50, 1200, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"obsidian.png"), std::wstring(L"obsidian"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 50, 1200, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"crying_obsidian.png"), std::wstring(L"crying_obsidian"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"bone_block_side.png"), std::wstring(L"bone_block"), stepBone, stepBone, stepBone, digBone, digBone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"netherrack.png"), std::wstring(L"netherrack"), stepNetherrack, stepNetherrack, stepNetherrack, digNetherrack, digNetherrack, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.8, 0.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"chiseled_quartz_block.png"), std::wstring(L"chiseled_quartz_block"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.8, 0.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"quartz_pillar.png"), std::wstring(L"quartz_pillar"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"quartz_block_bottom.png"), std::wstring(L"smooth_quartz_block"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.8, 0.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"quartz_bricks.png"), std::wstring(L"quartz_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"prismarine.png"), std::wstring(L"prismarine"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"prismarine_bricks.png"), std::wstring(L"prismarine_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"dark_prismarine.png"), std::wstring(L"dark_prismarine"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.25, 1.25, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"basalt_side.png"), std::wstring(L"basalt"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.25, 1.25, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"polished_basalt_side.png"), std::wstring(L"polished_basalt"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"blackstone.png"), std::wstring(L"blackstone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"polished_blackstone.png"), std::wstring(L"polished_blackstone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"chiseled_polished_blackstone.png"), std::wstring(L"chiseled_polished_blackstone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"gilded_blackstone.png"), std::wstring(L"gilded_blackstone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"magma.png"), std::wstring(L"magma_block"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollide, 0, 0, false, false, glowingLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 30, 30, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"ancient_debris_side.png"), std::wstring(L"ancient_debris"), breakAncientDebris, breakAncientDebris, breakAncientDebris, breakAncientDebris, breakAncientDebris, lightBlocking, { withPickaxe }, diamondHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"polished_blackstone_bricks.png"), std::wstring(L"polished_blackstone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cracked_polished_blackstone_bricks.png"), std::wstring(L"cracked_polished_blackstone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"bricks.png"), std::wstring(L"bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"nether_bricks.png"), std::wstring(L"nether_bricks"), stepNetherBrick, stepNetherBrick, stepNetherBrick, digNetherBrick, digNetherBrick, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"red_nether_bricks.png"), std::wstring(L"red_nether_bricks"), stepNetherBrick, stepNetherBrick, stepNetherBrick, digNetherBrick, digNetherBrick, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cracked_nether_bricks.png"), std::wstring(L"cracked_nether_bricks"), stepNetherBrick, stepNetherBrick, stepNetherBrick, digNetherBrick, digNetherBrick, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"chiseled_nether_bricks.png"), std::wstring(L"chiseled_nether_bricks"), stepNetherBrick, stepNetherBrick, stepNetherBrick, digNetherBrick, digNetherBrick, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"purpur_block.png"), std::wstring(L"purpur_block"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"purpur_pillar.png"), std::wstring(L"purpur_pillar"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"mossy_cobblestone.png"), std::wstring(L"mossy_cobblestone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"mossy_stone_bricks.png"), std::wstring(L"mossy_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cracked_stone_bricks.png"), std::wstring(L"cracked_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, blockList[blockID::mossy_stone_bricks]->tex, std::wstring(L"infested_mossy_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, blockList[blockID::cracked_stone_bricks]->tex, std::wstring(L"infested_cracked_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, blockList[blockID::stone]->tex, std::wstring(L"infested_stone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, blockList[blockID::cobblestone]->tex, std::wstring(L"infested_cobblestone"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, blockList[blockID::stone_bricks]->tex, std::wstring(L"infested_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, blockList[blockID::chiseled_stone_bricks]->tex, std::wstring(L"infested_chiseled_stone_bricks"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	// walls
	const wstringContainer wallNames = wstringContainer({
		std::wstring(L"end_stone_brick_wall"),
		std::wstring(L"brick_wall"),
		std::wstring(L"cobblestone_wall"),
		std::wstring(L"mossy_cobblestone_wall"),
		std::wstring(L"nether_brick_wall"),
		std::wstring(L"red_nether_brick_wall"),
		std::wstring(L"polished_blackstone_wall"),
		std::wstring(L"andesite_wall"),
		std::wstring(L"blackstone_wall"),
		std::wstring(L"diorite_wall"),
		std::wstring(L"granite_wall"),
		std::wstring(L"polished_blackstone_brick_wall"),
		std::wstring(L"prismarine_wall"),
		std::wstring(L"stone_brick_wall"),
		std::wstring(L"mossy_stone_brick_wall"),
		std::wstring(L"sandstone_wall"),
		std::wstring(L"red_sandstone_wall"),
		});

	const wstringContainer wallBlockNames = wstringContainer({
		std::wstring(L"end_stone_bricks"),
		std::wstring(L"bricks"),
		std::wstring(L"cobblestone"),
		std::wstring(L"mossy_cobblestone"),
		std::wstring(L"nether_bricks"),
		std::wstring(L"red_nether_bricks"),
		std::wstring(L"polished_blackstone"),
		std::wstring(L"andesite"),
		std::wstring(L"blackstone"),
		std::wstring(L"diorite"),
		std::wstring(L"granite"),
		std::wstring(L"polished_blackstone_bricks"),
		std::wstring(L"prismarine"),
		std::wstring(L"stone_bricks"),
		std::wstring(L"mossy_stone_bricks"),
		std::wstring(L"sandstone"),
		std::wstring(L"red_sandstone"),
		});

	for (size_t i = 0; i < wallBlockNames.size(); i++)
	{
		blockList[identifier] = new block((blockID)identifier, 6, 6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (wallBlockNames[i] + std::wstring(L".png"))), wallNames[i], stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}

	blockList[blockID::obsidian]->blockCollisionType = collisionTypeID::willCollideTop;
	blockList[blockID::purpur_pillar]->blockCollisionType = collisionTypeID::willCollideTop;
	blockList[blockID::quartz_pillar]->blockCollisionType = collisionTypeID::willCollideTop;

	// stairs

	wstringContainer stairNames =
		wstringContainer({ std::wstring(L"stone_stairs"),
						  std::wstring(L"granite_stairs"),
						  std::wstring(L"polished_granite_stairs"),
						  std::wstring(L"diorite_stairs"),
						  std::wstring(L"polished_diorite_stairs"),
						  std::wstring(L"andesite_stairs"),
						  std::wstring(L"polished_andesite_stairs"),
						  std::wstring(L"cobblestone_stairs"),
						  std::wstring(L"mossy_cobblestone_stairs"),
						  std::wstring(L"stone_brick_stairs"),
						  std::wstring(L"mossy_stone_brick_stairs"),
						  std::wstring(L"brick_stairs"),
						  std::wstring(L"end_stone_brick_stairs"),
						  std::wstring(L"nether_brick_stairs"),
						  std::wstring(L"red_nether_brick_stairs"),
						  std::wstring(L"sandstone_stairs"),
						  std::wstring(L"smooth_sandstone_stairs"),
						  std::wstring(L"red_sandstone_stairs"),
						  std::wstring(L"smooth_red_sandstone_stairs"),
						  std::wstring(L"quartz_stairs"),
						  std::wstring(L"smooth_quartz_stairs"),
						  std::wstring(L"purpur_stairs"),
						  std::wstring(L"prismarine_stairs"),
						  std::wstring(L"prismarine_brick_stairs"),
						  std::wstring(L"dark_prismarine_stairs"),
						  std::wstring(L"blackstone_stairs"),
						  std::wstring(L"polished_blackstone_stairs"),
						  std::wstring(L"polished_blackstone_brick_stairs") });

	wstringContainer stairTextureNames =
		wstringContainer({
			std::wstring(L"stone"),
			std::wstring(L"granite"),
			std::wstring(L"polished_granite"),
			std::wstring(L"diorite"),
			std::wstring(L"polished_diorite"),
			std::wstring(L"andesite"),
			std::wstring(L"polished_andesite"),
			std::wstring(L"cobblestone"),
			std::wstring(L"mossy_cobblestone"),
			std::wstring(L"stone_bricks"),
			std::wstring(L"mossy_stone_bricks"),
			std::wstring(L"bricks"),
			std::wstring(L"end_stone_bricks"),
			std::wstring(L"nether_bricks"),
			std::wstring(L"red_nether_bricks"),
			std::wstring(L"sandstone"),
			std::wstring(L"sand"),
			std::wstring(L"red_sandstone"),
			std::wstring(L"red_sand"),
			std::wstring(L"quartz_bricks"),
			std::wstring(L"quartz_block_bottom"),
			std::wstring(L"purpur_block"),
			std::wstring(L"prismarine"),
			std::wstring(L"prismarine_bricks"),
			std::wstring(L"dark_prismarine"),
			std::wstring(L"blackstone"),
			std::wstring(L"polished_blackstone"),
			std::wstring(L"polished_blackstone_bricks"),
			});

	// to get the planks textures
	blockList.update();

	for (int i = 0; i < woodTypeCount; i++)
	{
		resolutionTexture* tex = blockList[(int)getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->tex;

		blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, tex, woodTypeDataList[i]->name + std::wstring(L"_stairs"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount);
		identifier++;
	}
	for (size_t i = 0; i < stairTextureNames.size(); i++)
	{
		resolutionTexture* blockTexture = loadTextureFromResourcePack(blockTextureFolder / (stairTextureNames[i] + std::wstring(L".png")));

		blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, blockTexture, stairNames[i], stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}
	// slabs

	wstringContainer slabNames =
		wstringContainer({
			std::wstring(L"stone_slab"),
			std::wstring(L"smooth_stone_slab"),
			std::wstring(L"granite_slab"),
			std::wstring(L"polished_granite_slab"),
			std::wstring(L"diorite_slab"),
			std::wstring(L"polished_diorite_slab"),
			std::wstring(L"andesite_slab"),
			std::wstring(L"polished_andesite_slab"),
			std::wstring(L"cobblestone_slab"),
			std::wstring(L"mossy_cobblestone_slab"),
			std::wstring(L"stone_brick_slab"),
			std::wstring(L"mossy_stone_brick_slab"),
			std::wstring(L"brick_slab"),
			std::wstring(L"end_stone_brick_slab"),
			std::wstring(L"nether_brick_slab"),
			std::wstring(L"red_nether_brick_slab"),
			std::wstring(L"sandstone_slab"),
			std::wstring(L"cut_sandstone_slab"),
			std::wstring(L"smooth_sandstone_slab"),
			std::wstring(L"red_sandstone_slab"),
			std::wstring(L"cut_red_sandstone_slab"),
			std::wstring(L"smooth_red_sandstone_slab"),
			std::wstring(L"quartz_slab"),
			std::wstring(L"smooth_quartz_slab"),
			std::wstring(L"purpur_slab"),
			std::wstring(L"prismarine_slab"),
			std::wstring(L"prismarine_brick_slab"),
			std::wstring(L"dark_prismarine_slab"),
			std::wstring(L"blackstone_slab"),
			std::wstring(L"polished_blackstone_slab"),
			std::wstring(L"polished_blackstone_brick_slab"),
			});

	wstringContainer slabBlockNames =
		wstringContainer({
			std::wstring(L"stone"),
			std::wstring(L"smooth_stone"),
			std::wstring(L"granite"),
			std::wstring(L"polished_granite"),
			std::wstring(L"diorite"),
			std::wstring(L"polished_diorite"),
			std::wstring(L"andesite"),
			std::wstring(L"polished_andesite"),
			std::wstring(L"cobblestone"),
			std::wstring(L"mossy_cobblestone"),
			std::wstring(L"stone_bricks"),
			std::wstring(L"mossy_stone_bricks"),
			std::wstring(L"bricks"),
			std::wstring(L"end_stone_bricks"),
			std::wstring(L"nether_bricks"),
			std::wstring(L"red_nether_bricks"),
			std::wstring(L"sandstone"),
			std::wstring(L"cut_sandstone"),
			std::wstring(L"sand"),
			std::wstring(L"red_sandstone"),
			std::wstring(L"cut_red_sandstone"),
			std::wstring(L"red_sand"),
			std::wstring(L"quartz_bricks"),
			std::wstring(L"quartz_block_bottom"),
			std::wstring(L"purpur_block"),
			std::wstring(L"prismarine"),
			std::wstring(L"prismarine_bricks"),
			std::wstring(L"dark_prismarine"),
			std::wstring(L"blackstone"),
			std::wstring(L"polished_blackstone"),
			std::wstring(L"polished_blackstone_bricks"),
			});

	// to get the planks textures
	blockList.update();
	for (int i = 0; i < woodTypeCount; i++)
	{
		resolutionTexture* tex = blockList[(int)getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->tex;

		blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, tex, woodTypeDataList[i]->name + std::wstring(L"_slab"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount);
		identifier++;
	}
	for (size_t i = 0; i < slabBlockNames.size(); i++)
	{
		resolutionTexture* tex = loadTextureFromResourcePack(blockTextureFolder / (slabBlockNames[i] + std::wstring(L".png")));

		blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, tex, slabNames[i], stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}

	for (int i = 0; i < (int)colorID::count; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 0.8, 0.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (colorNames[i] + std::wstring(L"_wool.png"))), colorNames[i] + std::wstring(L"_wool"), stepCloth, stepCloth, stepCloth, stepCloth, stepCloth, lightFiltering, { withSwordOrShears }, noHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}
	for (int i = 0; i < (int)colorID::count; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 1.25, 1.25, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (colorNames[i] + std::wstring(L"_terracotta.png"))), colorNames[i] + std::wstring(L"_terracotta"), stepStone, stepStone, stepStone, digStone, digStone, sunlightPermeable, { withPickaxe }, woodHarvestTier);
		identifier++;
	}
	for (int i = 0; i < (int)colorID::count; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 1.4, 1.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (colorNames[i] + std::wstring(L"_glazed_terracotta.png"))), colorNames[i] + std::wstring(L"_glazed_terracotta"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"glass.png"), std::wstring(L"glass"), stepStone, stepStone, stepStone, breakGlass, digStone, noLightFilter, { withPickaxe }, noHarvestTier);
	identifier++;
	for (int i = 0; i < (int)colorID::count; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (colorNames[i] + std::wstring(L"_stained_glass.png"))), colorNames[i] + std::wstring(L"_stained_glass"), stepStone, stepStone, stepStone, breakGlass, digStone, lightFiltering, { withHand }, noHarvestTier);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, blockList[blockID::glass]->tex, std::wstring(L"glass_pane"), stepStone, stepStone, stepStone, breakGlass, digStone, noLightFilter, { withPickaxe }, noHarvestTier);
	identifier++;
	for (int i = 0; i < (int)colorID::count; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, blockList[(int)blockID::stained_glass + i]->tex, colorNames[i] + std::wstring(L"_stained_glass_pane"), stepStone, stepStone, stepStone, breakGlass, digStone, lightFiltering, { withHand }, noHarvestTier);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"fire_0.png"), std::wstring(L"fire"), extinguishSound, extinguishSound, extinguishSound, extinguishSound, extinguishSound, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, true, brightLightSource, experienceDrop(), fireSound);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"soul_fire_0.png"), std::wstring(L"soul_fire"), extinguishSound, extinguishSound, extinguishSound, extinguishSound, extinguishSound, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, true, moodyLightSource, experienceDrop(), fireSound);
	identifier++;

	for (int coralStateIndex = 0; coralStateIndex < (int)coralStateID::count; coralStateIndex++)
	{
		for (int coralColorIndex = 0; coralColorIndex < (int)coralColorID::count; coralColorIndex++)
		{
			for (int coralTypeIndex = 0; coralTypeIndex < (int)coralTypeID::count; coralTypeIndex++)
			{
				const std::wstring& name = coralStateNames[coralStateIndex] + coralColorNames[coralColorIndex] + coralTypeNames[coralTypeIndex];

				if (coralTypeIndex == (int)coralTypeID::coralBlock)
				{
					blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (name + std::wstring(L".png"))), name, stepCoral, stepCoral, stepCoral, breakCoral, breakCoral, lightBlocking, { withPickaxe }, woodHarvestTier);
					identifier++;
				}
				else
				{

					blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (name + std::wstring(L".png"))), name, stepWetGrass, stepWetGrass, digWetGrass, stepWetGrass, digWetGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
					identifier++;
				}
			}
		}
	}

	for (int i = 0; i < frostedIceMeltTreshold; i++)
	{
		frostedIceAgeTextures[i] = loadTextureFromResourcePack(blockTextureFolder / (std::wstring(L"frosted_ice_") + std::to_wstring(i) + std::wstring(L".png")));
	}

	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, frostedIceAgeTextures[0], std::wstring(L"frosted_ice"), stepStone, stepStone, stepStone, breakGlass, digStone, doubleFiltering, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"ice.png"), std::wstring(L"ice"), stepStone, stepStone, stepStone, breakGlass, digStone, doubleFiltering, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"packed_ice.png"), std::wstring(L"packed_ice"), stepStone, stepStone, stepStone, breakGlass, digStone, lightBlocking, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.8, 2.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"blue_ice.png"), std::wstring(L"blue_ice"), stepStone, stepStone, stepStone, breakGlass, digStone, lightBlocking, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollide, 0, 0, false, false, glowingLightSource);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"sponge.png"), std::wstring(L"sponge"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightBlocking, { withHoe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"wet_sponge.png"), std::wstring(L"wet_sponge"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightBlocking, { withHoe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"slime_block.png"), std::wstring(L"slime_block"), smallSlimeSound, smallSlimeSound, smallSlimeSound, bigSlimeSound, bigSlimeSound, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"honey_block_side.png"), std::wstring(L"honey_block"), stepHoney, stepHoney, stepHoney, breakHoney, breakHoney, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"mushroom_stem.png"), std::wstring(L"mushroom_stem"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"red_mushroom_block.png"), std::wstring(L"red_mushroom_block"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"brown_mushroom_block.png"), std::wstring(L"brown_mushroom_block"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"red_mushroom.png"), std::wstring(L"red_mushroom"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"brown_mushroom.png"), std::wstring(L"brown_mushroom"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, glowingLightSource);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 1, 1, 0.35, loadTextureFromResourcePack(blockTextureFolder / L"bamboo_stage0.png"), std::wstring(L"bamboo_shoot"), hitBambooSapling, hitBambooSapling, hitBambooSapling, placeBambooSapling, placeBambooSapling, lightBlocking, { withSwordOrShears }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1, 1, 0.35, loadTextureFromResourcePack(blockTextureFolder / L"bamboo_stalk.png"), std::wstring(L"bamboo"), stepBamboo, stepBamboo, stepBamboo, placeBamboo, placeBamboo, lightBlocking, { withSwordOrShears }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"clay.png"), std::wstring(L"clay"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, dirtTexture, std::wstring(L"dirt"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"coarse_dirt.png"), std::wstring(L"coarse_dirt"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"bonebed_dirt.png"), std::wstring(L"bonebed_dirt"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.65, 0.65, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"grass_path_side.png"), std::wstring(L"grass_path"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"grass_block_side.png"), std::wstring(L"grass_block"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"podzol_side.png"), std::wstring(L"podzol"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"mycelium_side.png"), std::wstring(L"mycelium"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"crimson_nylium_side.png"), std::wstring(L"crimson_nylium"), stepNylium, stepNylium, stepNylium, digNylium, digNylium, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"warped_nylium_side.png"), std::wstring(L"warped_nylium"), stepNylium, stepNylium, stepNylium, digNylium, digNylium, lightBlocking, { withPickaxe }, woodHarvestTier);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"sea_lantern.png"), std::wstring(L"sea_lantern"), stepStone, stepStone, stepStone, breakGlass, digStone, lightBlocking, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, glowingLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"glowstone.png"), std::wstring(L"glowstone"), stepStone, stepStone, stepStone, breakGlass, digStone, lightBlocking, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, brightLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1, 1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"shroomlight.png"), std::wstring(L"shroomlight"), stepShroomLight, stepShroomLight, stepShroomLight, digShroomLight, digShroomLight, lightBlocking, { withHoe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, moodyLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.3, 0.3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"redstone_lamp.png"), std::wstring(L"redstone_lamp"), stepStone, stepStone, stepStone, breakGlass, digStone, lightBlocking, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"crafting_table_front.png"), std::wstring(L"crafting_table"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cartography_table_side2.png"), std::wstring(L"cartography_table"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"smithing_table_side.png"), std::wstring(L"smithing_table"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"loom_front.png"), std::wstring(L"loom"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"furnace_front.png"), std::wstring(L"furnace"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"blast_furnace_front.png"), std::wstring(L"blast_furnace"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"smoker_front.png"), std::wstring(L"smoker"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"barrel_side.png"), std::wstring(L"barrel"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadChestTexture(entityTextureFolder / L"chest" / L"normal.png"), std::wstring(L"chest"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadChestTexture(entityTextureFolder / L"chest" / L"trapped.png"), std::wstring(L"trapped_chest"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 22.5, 22.5, standardBlockWeightPerCubicMeter, loadChestTexture(entityTextureFolder / L"chest" / L"ender.png"), std::wstring(L"ender_chest"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, moodyLightSource);
	identifier++;

	resolutionTexture* furnaceSideImage = loadTextureFromResourcePack(blockTextureFolder / L"furnace_side.png");
	resolutionTexture* furnaceTopImage = loadTextureFromResourcePack(blockTextureFolder / L"furnace_top.png");

	const wstringContainer& dispenserNames = { std::wstring(L"dispenser"), std::wstring(L"dropper") };
	for (size_t i = 0; i < dispenserNames.size(); i++)
	{
		resolutionTexture* frontHorizontalImage = loadTextureFromResourcePack(blockTextureFolder / (dispenserNames[i] + std::wstring(L"_front.png")));
		resolutionTexture* frontVerticalImage = loadTextureFromResourcePack(blockTextureFolder / (dispenserNames[i] + std::wstring(L"_front_vertical.png")));

		resolutionTexture* horizontalTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));
		resolutionTexture* verticalTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));

		cfp dispenserFrontPart = 0.25;

		fillTransformedTexture(crectangle2(blockTextureRect.x, blockTextureRect.y, blockTextureRect.w * dispenserFrontPart, blockTextureRect.h), *frontHorizontalImage, *horizontalTexture);
		fillTransformedTexture(crectangle2(blockTextureRect.w * dispenserFrontPart, blockTextureRect.y, blockTextureRect.w * (1 - dispenserFrontPart), blockTextureRect.h), *furnaceSideImage, *horizontalTexture);

		fillTransformedTexture(crectangle2(blockTextureRect.x, blockTextureRect.y, blockTextureRect.w, blockTextureRect.h * (1 - dispenserFrontPart)), *furnaceTopImage, *verticalTexture);
		fillTransformedTexture(crectangle2(blockTextureRect.x, blockTextureRect.h * (1 - dispenserFrontPart), blockTextureRect.w, blockTextureRect.h * dispenserFrontPart), *frontVerticalImage, *verticalTexture);

		verticalDispenserTextures[i] = verticalTexture;

		blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, horizontalTexture, dispenserNames[i], stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 5, 5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"enchanting_table_side.png"), std::wstring(L"enchanting_table"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, diamondHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	resolutionTexture* basicAnvilTexture = loadTextureFromResourcePack(blockTextureFolder / L"anvil.png");

	std::vector<rectangle2> relativeAnvilRects{
		crectangle2(0.1, 0, 0.8, 0.2),
		crectangle2(0.2, 0.2, 0.6, 0.1),
		crectangle2(0.25, 0.3, 0.5, 0.3),
		crectangle2(0, 0.6, 1, 0.4) };

	const wstringContainer& anvilTextureNames = { std::wstring(L"anvil_top"), std::wstring(L"chipped_anvil_top"), std::wstring(L"damaged_anvil_top") };

	for (int i = 0; i < anvilDamageLevelCount; i++)
	{
		cfsize_t& anvilTextureSize = basicAnvilTexture->scaledTextures[0]->size.x;
		crectangle2& anvilTextureRect = basicAnvilTexture->getClientRect();
		resolutionTexture* anvilGraphics = new resolutionTexture(texture(cvect2<fsize_t>(anvilTextureSize)), cvec2(blockTextureSize));

		csize_t& lastRectIndex = relativeAnvilRects.size() - 1;
		for (size_t anvilRectIndex = 0; anvilRectIndex < lastRectIndex; anvilRectIndex++)
		{
			crectangle2& blockRect = getAbsoluteRect(anvilTextureRect, relativeAnvilRects[anvilRectIndex]);

			fillTransformedBrushRectangle(blockRect, blockRect, *basicAnvilTexture, *anvilGraphics);
		}

		crectangle2& damageablePartRect = getAbsoluteRect(anvilTextureRect, relativeAnvilRects[lastRectIndex]);

		resolutionTexture* damageablePartTexture = loadTextureFromResourcePack(blockTextureFolder / (anvilTextureNames[i] + std::wstring(L".png")), false);

		fillTransparentRectangle(crectangle2(3, 0, 10, 0x10), 90, damageablePartRect, *damageablePartTexture, *anvilGraphics);

		delete damageablePartTexture;

		anvilGraphics->recalculateScaledTextures();

		anvilTextures[i] = anvilGraphics;
		loadedTextures.push_back(anvilGraphics);
	}
	blockList[identifier] = new block((blockID)identifier, 5, 1200, standardBlockWeightPerCubicMeter, anvilTextures[0], std::wstring(L"anvil"), stepStone, stepStone, stepStone, digStone, anvilLandSound, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	auto grindStoneTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));

	resolutionTexture* grindStoneSideTexture = loadTextureFromResourcePack(blockTextureFolder / L"grindstone_side.png", false);
	resolutionTexture* grindStonePivotTexture = loadTextureFromResourcePack(blockTextureFolder / L"grindstone_pivot.png", false);

	crectangle2 grindStoneSideTextureRect = crectangle2(0, 4, 12, 12);
	crectangle2 grindStonePivotTopTextureRect = crectangle2(0, 10, 6, 6);
	crectangle2 grindStonePivotBottomTextureRect = crectangle2(6, 10, 4, 6);

	cvec2& grindStoneSidePosition = (cveci2(blockTextureSize) - cveci2(grindStoneSideTextureRect.size)) / 2;
	cvec2& grindStonePivotTopPosition = cveci2((blockTextureSize - (int)grindStonePivotTopTextureRect.size.x) / 2, (int)grindStonePivotBottomTextureRect.h) / 2;
	cvec2& grindStonePivotBottomPosition = cveci2((blockTextureSize - (int)grindStonePivotBottomTextureRect.size.x) / 2, 0);

	fillTransformedBrushRectangle(grindStoneSideTextureRect, grindStoneSidePosition, *grindStoneSideTexture, *grindStoneTexture);
	fillTransformedBrushRectangle(grindStonePivotBottomTextureRect, grindStoneSidePosition, *grindStoneSideTexture, *grindStoneTexture);
	fillTransformedBrushRectangle(grindStonePivotTopTextureRect, grindStoneSidePosition, *grindStoneSideTexture, *grindStoneTexture);
	fillTransformedBrushRectangle(grindStoneSideTextureRect, grindStoneSidePosition, *grindStoneSideTexture, *grindStoneTexture);
	fillTransformedBrushRectangle(grindStonePivotBottomTextureRect, grindStonePivotBottomPosition, *grindStonePivotTexture, *grindStoneTexture);
	fillTransformedBrushRectangle(grindStonePivotTopTextureRect, grindStonePivotTopPosition, *grindStonePivotTexture, *grindStoneTexture);

	delete grindStoneSideTexture;
	delete grindStonePivotTexture;
	loadedTextures.push_back(grindStoneTexture);

	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, grindStoneTexture, std::wstring(L"grindstone"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"fletching_table_side.png"), std::wstring(L"fletching_table"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	resolutionTexture* innerBeaconTexture = loadTextureFromResourcePack(blockTextureFolder / L"beacon.png");

	constexpr rectangle2 innerBeaconTextureRect = crectangle2(2, 2, 28, 28);

	constexpr rectangle2 beaconObsidianTextureRect = crectangle2(0, 0, blockTextureSize, innerBeaconTextureRect.getY());

	resolutionTexture* beaconTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));

	fillTransformedBrushRectangle(innerBeaconTextureRect, innerBeaconTextureRect.pos0, *innerBeaconTexture, *beaconTexture);

	fillTransformedBrushRectangle(beaconObsidianTextureRect, beaconObsidianTextureRect.pos0, *blockList[blockID::obsidian]->tex, *beaconTexture);

	fillTransparentRectangle(crectangle2(blockTextureRect), *blockList[blockID::glass]->tex, *beaconTexture);

	blockList[identifier] = new block((blockID)identifier, 3, 3, standardBlockWeightPerCubicMeter, beaconTexture, std::wstring(L"beacon"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, brightLightSource);
	identifier++;

	resolutionTexture* conduitTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));

	resolutionTexture* unOpenedConduitTexture = loadTextureFromResourcePack(entityTextureFolder / L"conduit" / L"base.png");

	constexpr rectangle2 unOpenedConduitRect = crectangle2(cveci2(4), cveci2(12));

	fillTransformedBrushRectangle(unOpenedConduitRect, (cveci2(blockTextureSize) - unOpenedConduitRect.size) / 2, *unOpenedConduitTexture, *conduitTexture);

	blockList[identifier] = new block((blockID)identifier, 3, 3, standardBlockWeightPerCubicMeter, conduitTexture, std::wstring(L"conduit"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, brightLightSource);
	identifier++;

	resolutionTexture* lecternSidesTexture = loadTextureFromResourcePack(blockTextureFolder / L"lectern_sides.png");

	resolutionTexture* lecternGraphics = new resolutionTexture(texture(cvect2<fsize_t>((fsize_t)(blockTextureSize * lecternSidesTexture->getScaleModifier()))), cvec2(blockTextureSize));

	constexpr rectangle2 lecternPoleTextureRect = crectangle2(4, 0, 8, 12);

	constexpr rectangle2 lecternTopTextureRect = crectangle2(0, 12, blockTextureSize, 0x4);
	constexpr rectangle2 lecternBottomTextureRect = crectangle2(0, 0x8, blockTextureSize, 0x4);

	constexpr fp lecternMiddlePixelHeight = lecternPoleTextureRect.size.getY();

	constexpr rectangle2 lecternMiddleTextureRect = crectangle2(blockTextureSize - lecternMiddlePixelHeight, 0, lecternMiddlePixelHeight, 0x8);

	fillTransparentRectangle(crectangle2(lecternMiddleTextureRect), 90, lecternPoleTextureRect, *lecternSidesTexture, *lecternGraphics);

	fillTransformedBrushRectangle(lecternBottomTextureRect, cveci2(), *lecternSidesTexture, *lecternGraphics);

	mat3x3 rotationTransform = cmat3x3::rotate(crectangle2(lecternTopTextureRect).getCenter(), -20 * math::degreesToRadians);

	crectangle2 rotatedRectangleBounds = rotationTransform.getTransformedRectangleBounds(crectangle2(lecternTopTextureRect));

	// translate lower and rotate 20 degrees
	cmat3x3 transform = mat3x3::cross(cmat3x3::translate(cvec2(0, blockTextureSize - (rotatedRectangleBounds.y + rotatedRectangleBounds.h))), rotationTransform);

	fillTransformedBrushRectangle(crectangle2(lecternTopTextureRect), transform, *lecternSidesTexture, *lecternGraphics);

	blockList[identifier] = new block((blockID)identifier, 2.5, 2.5, standardBlockWeightPerCubicMeter, lecternGraphics, std::wstring(L"lectern"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"composter_side.png"), std::wstring(L"composter"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cake_side.png"), std::wstring(L"cake"), stepCloth, stepCloth, stepCloth, stepCloth, stepCloth, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"scaffolding_side.png"), std::wstring(L"scaffolding"), stepScaffolding, stepScaffolding, stepScaffolding, placeScaffolding, placeScaffolding, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	for (int i = 0; i <= respawnAnchorMaximumCharge; i++)
	{
		respawnAnchorTextures[i] = loadTextureFromResourcePack(blockTextureFolder / (std::wstring(L"respawn_anchor_side") + std::to_wstring(i) + std::wstring(L".png")));
	}

	blockList[identifier] = new block((blockID)identifier, 50, 1200, standardBlockWeightPerCubicMeter, respawnAnchorTextures[0], std::wstring(L"respawn_anchor"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, diamondHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"target_side.png"), std::wstring(L"target"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withHoe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"lodestone_side.png"), std::wstring(L"lodestone"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 5, 5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"bell.png"), std::wstring(L"bell"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"torch.png"), std::wstring(L"torch"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, moodyLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"soul_torch.png"), std::wstring(L"soul_torch"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, moodyLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"ladder.png"), std::wstring(L"ladder"), stepLadder, stepLadder, stepLadder, digWood, digWood, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, farmlandTexture, std::wstring(L"farmland"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel });
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.1, 0.1, standardBlockWeightPerCubicMeter, snowTexture, std::wstring(L"snow"), stepSnow, stepSnow, stepSnow, stepSnow, stepSnow, lightFiltering, { withShovel }, woodHarvestTier, collisionTypeID::willCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, snowTexture, std::wstring(L"snow_block"), stepSnow, stepSnow, stepSnow, stepSnow, stepSnow, sunlightPermeable, { withShovel }, woodHarvestTier);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, lanternGraphics, std::wstring(L"lantern"), placeLantern, placeLantern, breakLantern, breakLantern, placeLantern, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, brightLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, lanternGraphics, std::wstring(L"soul_lantern"), placeLantern, placeLantern, breakLantern, breakLantern, placeLantern, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, moodyLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"sea_pickle.png"), std::wstring(L"sea_pickle"), smallSlimeSound, smallSlimeSound, smallSlimeSound, bigSlimeSound, bigSlimeSound, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, glowingLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"flower_pot.png"), std::wstring(L"flower_pot"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 5, 5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"chain.png"), std::wstring(L"chain"), stepChain, stepChain, stepChain, breakChain, breakChain, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"turtle_egg.png"), std::wstring(L"turtle_egg"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"bookshelf.png"), std::wstring(L"bookshelf"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"hay_block_side.png"), std::wstring(L"hay_block"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withHoe }, noHarvestTier, collisionTypeID::willCollideTop, 60, 20, false);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"tnt_side.png"), std::wstring(L"tnt"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withHoe }, noHarvestTier, collisionTypeID::willCollideTop, 60, 20, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 4, 4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cobweb.png"), std::wstring(L"cobweb"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withSwordOrShears }, woodHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, brewingStandBaseTexture, std::wstring(L"brewing_stand"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, glowingLightSource);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"cauldron.png"), std::wstring(L"cauldron"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 3, 3, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"hopper_outside.png"), std::wstring(L"hopper"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, redStoneWireTexture, std::wstring(L"redstone_wire"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"redstone_torch.png"), std::wstring(L"redstone_torch"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"repeater.png"), std::wstring(L"repeater"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"comparator.png"), std::wstring(L"comparator"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	for (int i = 0; i < woodTypeCount; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, blockList[getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->tex, woodTypeDataList[i]->name + std::wstring(L"_button"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, blockList[blockID::stone]->tex, std::wstring(L"stone_button"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, blockList[blockID::polished_blackstone]->tex, std::wstring(L"polished_blackstone_button"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	for (int i = 0; i < woodTypeCount; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, blockList[getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->tex, woodTypeDataList[i]->name + std::wstring(L"_pressure_plate"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, blockList[blockID::stone]->tex, std::wstring(L"stone_pressure_plate"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, blockList[blockID::polished_blackstone]->tex, std::wstring(L"polished_blackstone_pressure_plate"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, blockList[blockID::gold_block]->tex, std::wstring(L"light_weighted_pressure_plate"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, blockList[blockID::iron_block]->tex, std::wstring(L"heavy_weighted_pressure_plate"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"lever.png"), std::wstring(L"lever"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withAxe }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"daylight_detector_top.png"), std::wstring(L"daylight_detector"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"observer_side.png"), std::wstring(L"observer"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.8, 0.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"note_block.png"), std::wstring(L"note_block"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 10, 10, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 2, 2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"jukebox_side.png"), std::wstring(L"jukebox"), stepWood, stepWood, stepWood, digWood, digWood, lightBlocking, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, 10, 10, true);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"tripwire.png"), std::wstring(L"tripwire"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 3.5, 3.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"tripwire_hook.png"), std::wstring(L"tripwire_hook"), stepStone, stepStone, stepStone, digStone, digStone, noLightFilter, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	const wstringContainer pistonNames = { std::wstring(L"piston"), std::wstring(L"sticky_piston") };
	const wstringContainer pistonTopTextureNames = { std::wstring(L"piston_top"), std::wstring(L"piston_top_sticky") };

	resolutionTexture* pistonSideTexture = loadTextureFromResourcePack(blockTextureFolder / L"piston_side.png", false);
	for (int i = 0; i < 2; i++)
	{
		resolutionTexture* pistonTopTexture = loadTextureFromResourcePack(blockTextureFolder / (pistonTopTextureNames[i] + std::wstring(L".png")), false);
		cfp& scaleModifier = pistonTopTexture->getScaleModifier();
		resolutionTexture* pistonTexture = new resolutionTexture(texture(cveci2((int)(blockTextureSize * scaleModifier), (int)(blockTextureSize * scaleModifier * 2))), cvec2(blockTextureSize, blockTextureSize * 2));

		// bottom
		fillTransformedBrushRectangle(crectangle2(0, 0, blockTextureSize, blockTextureSize - pistonTopPixelSize), cveci2(), *pistonSideTexture, *pistonTexture);

		// arm
		fillTransparentRectangle(crectangle2(0, blockTextureSize - pistonTopPixelSize, blockTextureSize, pistonTopPixelSize), 90, crectangle2((blockTextureSize - pistonTopPixelSize) / 2, blockTextureSize - pistonTopPixelSize, pistonTopPixelSize, blockTextureSize), *pistonSideTexture, *pistonTexture);
		// fillTransformedBrushRectangle(crectangle2(0, 0, blockTextureSize, blockTextureSize - pistonTopPixelSize), cveci2(), *pistonSideTexture, *pistonTexture);

		// top
		fillTransformedBrushRectangle(crectangle2(0, 0, blockTextureSize, pistonTopPixelSize), cvec2(0, pistonTexture->defaultSize.y - pistonTopPixelSize), *pistonTopTexture, *pistonTexture);

		blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, pistonTexture, pistonNames[i], stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
		delete pistonTopTexture;
	}
	delete pistonSideTexture;

	blockList[identifier] = new block((blockID)identifier, 0.7, 0.7, standardBlockWeightPerCubicMeter, loadRailsTexture(blockTextureFolder / L"rail.png"), std::wstring(L"rail"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.7, 0.7, standardBlockWeightPerCubicMeter, loadRailsTexture(blockTextureFolder / L"powered_rail.png"), std::wstring(L"powered_rail"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.7, 0.7, standardBlockWeightPerCubicMeter, loadRailsTexture(blockTextureFolder / L"detector_rail.png"), std::wstring(L"detector_rail"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.7, 0.7, standardBlockWeightPerCubicMeter, loadRailsTexture(blockTextureFolder / L"activator_rail.png"), std::wstring(L"activator_rail"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"nether_portal.png"), std::wstring(L"nether_portal"), stepStone, stepStone, stepStone, breakGlass, digStone, noLightFilter, { withPickaxe }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, brightLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(entityTextureFolder / L"end_portal.png"), std::wstring(L"end_portal"), stepStone, stepStone, stepStone, breakGlass, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willNotCollide, 0, 0, false, false, brightLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"end_portal_frame_side.png"), std::wstring(L"end_portal_frame"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe });
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 5, 5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"spawner.png"), std::wstring(L"spawner"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, glowingLightSource, experienceDrop(15, 43));
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"dead_bush.png"), std::wstring(L"dead_bush"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withSwordOrShears }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"short_grass.png"), std::wstring(L"short_grass"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"fern.png"), std::wstring(L"fern"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true);
	identifier++;
	blockList[identifier] = new doubleBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, tallGrassTopTexture, std::wstring(L"tall_grass"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true), loadTextureFromResourcePack(blockTextureFolder / L"tall_grass_bottom.png"), tallGrassTopTexture, cveci2(0, 1));
	identifier++;
	blockList[identifier] = new doubleBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, largeFernTopTexture, std::wstring(L"large_fern"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true), loadTextureFromResourcePack(blockTextureFolder / L"large_fern_bottom.png"), largeFernTopTexture, cveci2(0, 1));
	identifier++;

	for (int i = 0; i < netherTreeTypeCount; i++)
	{
		const std::wstring& name = woodTypeDataList[(int)woodTypeID::crimson + i]->name + std::wstring(L"_roots");
		blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (name + std::wstring(L".png"))), name, breakRoots, stepRoots, breakRoots, breakRoots, breakRoots, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"vine.png"), std::wstring(L"vine"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withSwordOrShears }, ironHarvestTier, collisionTypeID::willNotCollide, 15, 15, true);
	identifier++;

	for (int i = 0; i < (int)netherVineTypeID::count; i++)
	{
		const std::wstring& name = netherVineTypeDataList[i]->name + std::wstring(L"_vines");
		const std::wstring& plantName = name + std::wstring(L"_plant");
		blockList[identifier] = new connectedBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (plantName + std::wstring(L".png"))), name, breakRoots, stepRoots, breakRoots, breakRoots, breakRoots, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide), loadTextureFromResourcePack(blockTextureFolder / (name + std::wstring(L".png"))), (i == 0) ? cveci2(0, -1) : cveci2(0, 1));
		identifier++;
	}

	const wstringContainer flowerNames =
		wstringContainer({
			std::wstring(L"dandelion"),
			std::wstring(L"poppy"),
			std::wstring(L"blue_orchid"),
			std::wstring(L"allium"),
			std::wstring(L"azure_bluet"),
			std::wstring(L"red_tulip"),
			std::wstring(L"pink_tulip"),
			std::wstring(L"white_tulip"),
			std::wstring(L"orange_tulip"),
			std::wstring(L"oxeye_daisy"),
			std::wstring(L"cornflower"),
			std::wstring(L"lily_of_the_valley"),
			std::wstring(L"wither_rose"),
			std::wstring(L"sunflower"),
			std::wstring(L"lilac"),
			std::wstring(L"rose_bush"),
			std::wstring(L"peony"),
			});

	for (int i = 0; i < flowerTypeCount; i++)
	{
		if (isDoubleBlock((blockID)identifier))
		{
			resolutionTexture* topTexture = loadTextureFromResourcePack(blockTextureFolder / (flowerNames[i] + std::wstring(L"_top.png")));
			blockList[identifier] = new doubleBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, topTexture, flowerNames[i], stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true), loadTextureFromResourcePack(blockTextureFolder / (flowerNames[i] + std::wstring(L"_bottom.png"))), topTexture, cveci2(0, 1));
			identifier++;
		}
		else
		{
			blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (flowerNames[i] + std::wstring(L".png"))), flowerNames[i], stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true);
			identifier++;
		}
	}

	const wstringContainer sandStoneColorNames = { std::wstring(), std::wstring(L"red_") };
	const wstringContainer sandStoneTypeNames = { std::wstring(), std::wstring(L"chiseled_"), std::wstring(L"cut_"), std::wstring(L"smooth_") };
	for (int i = 0; i < (int)sandStoneColorID::count; i++)
	{
		for (int j = 0; j < (int)sandStoneTypeID::count; j++)
		{
			const std::wstring name = sandStoneTypeNames[j] + sandStoneColorNames[i] + std::wstring(L"sandstone");
			cfp hardness = j == (int)sandStoneTypeID::smooth ? 2 : 0.8;
			const std::wstring textureName = j == (int)sandStoneTypeID::smooth ? sandStoneColorNames[i] + std::wstring(L"sand") : name;
			blockList[identifier] = new block((blockID)identifier, hardness, hardness, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (textureName + std::wstring(L".png"))), name, stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
			identifier++;
		}
	}

	for (int i = 0; i < (int)colorID::count; i++)
	{
		resolutionTexture* bedTexture = loadTextureFromResourcePack(entityTextureFolder / L"bed" / (colorNames[i] + std::wstring(L".png")));
		auto backTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));
		auto frontTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));

		// cmat3x3 bedBackMatrix = mat3x3::cross(mat3x3::mirror(axisID::x, blockTextureSize / 2), mat3x3::fromRectToRotatedRect(bedBackTextureRect, 270, crectangle2(0, bedLegTextureRect.h, bedBackTextureRect.h, bedBackTextureRect.w)));
		cmat3x3 bedBackMatrix = mat3x3::fromRectToRotatedRect(bedBackTextureRect, 270, crectangle2(0, bedLegTextureRect.h, bedBackTextureRect.h, bedBackTextureRect.w));
		fillTransformedBrushRectangle(bedBackTextureRect, bedBackMatrix, *bedTexture, *backTexture);

		// cmat3x3 bedFrontMatrix = mat3x3::cross(mat3x3::mirror(axisID::x, blockTextureSize / 2), mat3x3::fromRectToRotatedRect(bedFrontTextureRect, 270, crectangle2(0, bedLegTextureRect.h, bedFrontTextureRect.h, bedFrontTextureRect.w)));
		cmat3x3 bedFrontMatrix = mat3x3::fromRectToRotatedRect(bedFrontTextureRect, 270, crectangle2(0, bedLegTextureRect.h, bedFrontTextureRect.h, bedFrontTextureRect.w));
		fillTransformedBrushRectangle(bedFrontTextureRect, bedFrontMatrix, *bedTexture, *frontTexture);

		fillTransformedBrushRectangle(bedLegTextureRect, crectangle2(blockTextureSize - bedLegTextureRect.w, 0, bedLegTextureRect.w, bedLegTextureRect.h), *bedTexture, *backTexture);
		fillTransformedBrushRectangle(bedLegTextureRect, crectangle2(0, 0, bedLegTextureRect.w, bedLegTextureRect.h), *bedTexture, *frontTexture);

		blockList[identifier] = new doubleBlock(block((blockID)identifier, 0.2, 0.2, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / (colorNames[i] + std::wstring(L"_bed.png"))), colorNames[i] + std::wstring(L"_bed"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop), backTexture, frontTexture, cveci2(-1, 0));
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"campfire.png"), std::wstring(L"campfire"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(itemTextureFolder / L"soul_campfire.png"), std::wstring(L"soul_campfire"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"end_rod.png"), std::wstring(L"end_rod"), stepWood, stepWood, stepWood, digWood, digWood, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, brightLightSource);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"chorus_plant.png"), std::wstring(L"chorus_plant"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	resolutionTexture* chorusFlowerTexture = loadTextureFromResourcePack(blockTextureFolder / L"chorus_flower.png");
	blockList[identifier] = new cropBlock(block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, chorusFlowerTexture, std::wstring(L"chorus_flower"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop), 6, std::vector<resolutionTexture*>({ chorusFlowerTexture, chorusFlowerTexture, chorusFlowerTexture, chorusFlowerTexture, chorusFlowerTexture, loadTextureFromResourcePack(blockTextureFolder / L"chorus_flower_dead.png") }), 1);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.4, 0.4, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"cactus_side.png"), std::wstring(L"cactus"), stepCloth, stepCloth, stepCloth, stepCloth, stepCloth, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"sugar_cane.png"), std::wstring(L"sugar_cane"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	blockList[identifier] = new connectedBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"kelp_plant.png"), std::wstring(L"kelp"), stepWetGrass, stepWetGrass, digWetGrass, stepWetGrass, digWetGrass, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide), loadTextureFromResourcePack(blockTextureFolder / L"kelp.png"), cveci2(0, 1));
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"dried_kelp_side.png"), std::wstring(L"dried_kelp_block"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, { withHoe }, noHarvestTier, collisionTypeID::willNotCollide);
	identifier++;
	// blockList[identifier] = new block((blockID)id, 0, loadTextureFromResourcePack(blockTextureFolder / L"white_bed.png"), std::wstring(L"sugar_cane"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, lightFiltering, {withHand}, new itemDrop((itemID)id),noToolTier, collisionTypeID::willNotCollide)); id++;

	wstringContainer cropNames = wstringContainer({ std::wstring(L"wheat"), std::wstring(L"carrots"), std::wstring(L"potatoes"), std::wstring(L"beetroots"), std::wstring(L"nether_wart") });
	cint cropsToAdd = 5;

	cint growthStageTextureCount[cropsToAdd]{
		0x8,
		0x4,
		0x4,
		0x4,
		0x3,
	};
	cint growthStageCount[cropsToAdd]{
		0x8,
		0x8,
		0x8,
		0x4,
		0x4,
	};

	const std::vector<int> growthStageTextureIndexes[cropsToAdd]{
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 0, 1, 1, 2, 2, 2, 3},
		{0, 0, 1, 1, 2, 2, 2, 3},
		{0, 1, 2, 3},
		{0, 1, 1, 2},
	};

	cfp chanceToGrow[cropsToAdd]{
		0.5,
		0.3,
		0.3,
		0.6,
		0.1,
	};

	for (int i = 0; i < cropsToAdd; i++)
	{
		auto growthStageTextures = std::vector<resolutionTexture*>();

		for (int stageTextureIndex = 0; stageTextureIndex < growthStageTextureCount[i]; stageTextureIndex++)
		{
			growthStageTextures.push_back(loadTextureFromResourcePack(blockTextureFolder / (cropNames[i] + std::wstring(L"_stage") + std::to_wstring(stageTextureIndex) + std::wstring(L".png"))));
		}
		auto linkedGrowthStageTextures = std::vector<resolutionTexture*>();
		for (int stageIndex = 0; stageIndex < growthStageCount[i]; stageIndex++)
		{
			linkedGrowthStageTextures.push_back(growthStageTextures[growthStageTextureIndexes[i][stageIndex]]);
		}

		std::shared_ptr<audioCollection> breakSound = identifier == (int)blockID::nether_wart ? breakNetherWart : breakCrop;
		std::shared_ptr<audioCollection> stepSound = identifier == (int)blockID::nether_wart ? stepNetherWart : stepGrass;

		blockList[identifier] = new cropBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, growthStageTextures[growthStageTextureIndexes[i][0]], cropNames[i], stepSound, stepSound, stepSound, breakSound, breakSound, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide), growthStageCount[i], linkedGrowthStageTextures, chanceToGrow[i]);
		identifier++;
	}
	blockList[blockID::wheat]->tex = loadTextureFromResourcePack(itemTextureFolder / L"wheat.png");

	blockList[blockID::nether_wart]->tex = loadTextureFromResourcePack(itemTextureFolder / L"nether_wart.png");

	cint sweetBerryBushGrowthStageCount = 4;
	std::vector<resolutionTexture*> sweetBerriesGrowthStageTextures = loadGrowthStageTextures(std::wstring(L"sweet_berry_bush"), { 0ULL, 1ULL, 2ULL, 3ULL });
	blockList[identifier] = new cropBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, sweetBerriesGrowthStageTextures[sweetBerryBushGrowthStageCount - 1], std::wstring(L"sweet_berry_bush"), stepGrass, stepGrass, stepGrass, breakBerryBush, placeBerryBush, lightFiltering, { withHand }, noHarvestTier, collisionTypeID::willNotCollide), sweetBerryBushGrowthStageCount, sweetBerriesGrowthStageTextures, (defaultTicksPerRandomTick * sweetBerryBushGrowthStageCount) / (ticksPerDay * 0.5));
	identifier++;

	const std::wstring stemPlantNames[stemPlantTypeCount]{
		std::wstring(L"melon"),
		std::wstring(L"pumpkin") };
	for (int i = 0; i < stemPlantTypeCount; i++)
	{
		unAttachedStemTextures[i] = loadTextureFromResourcePack(blockTextureFolder / (stemPlantNames[i] + std::wstring(L"_stem.png")));
		attachedStemTextures[i] = loadTextureFromResourcePack(blockTextureFolder / (std::wstring(L"attached_") + stemPlantNames[i] + std::wstring(L"_stem.png")));
	}

	for (int i = 0; i < stemPlantTypeCount; i++)
	{
		blockList[identifier] = new cropBlock(block((blockID)identifier, 0, 0, standardBlockWeightPerCubicMeter, unAttachedStemTextures[i], stemPlantNames[i] + std::wstring(L"_stem"), stepWood, stepWood, stepWood, breakCrop, breakCrop, noLightFilter, { withAxe }, noHarvestTier, collisionTypeID::willNotCollide), stemPlantGrowthStageCount[i], std::vector<resolutionTexture*>(8, unAttachedStemTextures[i]), stemPlantChanceToGrow[i]);
		identifier++;
	}
	for (int i = 0; i < stemPlantTypeCount; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 1, 1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (stemPlantNames[i] + std::wstring(L"_side.png"))), stemPlantNames[i], stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop);
		identifier++;
	}
	blockList[identifier] = new block((blockID)identifier, 1, 1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"carved_pumpkin.png"), std::wstring(L"carved_pumpkin"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollide);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, 1, 1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"jack_o_lantern.png"), std::wstring(L"jack_o_lantern"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollide, 0, 0, false, false, moodyLightSource);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"soul_soil.png"), std::wstring(L"soul_soil"), stepSoulSoil, stepSoulSoil, stepSoulSoil, breakSoulSoil, breakSoulSoil, lightBlocking, { withShovel });
	identifier++;

	for (int i = 0; i < (int)colorID::count; i++)
	{
		const std::wstring& name = colorNames[i] + std::wstring(L"_concrete");
		blockList[identifier] = new block((blockID)identifier, 1.8, 1.8, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (name + std::wstring(L".png"))), name, stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
		identifier++;
	}

	for (int i = 0; i < (int)colorID::count; i++)
	{
		const std::wstring& name = colorNames[i] + std::wstring(L"_concrete_powder");
		blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (name + std::wstring(L".png"))), name, stepSand, stepSand, stepSand, digSand, digSand, lightBlocking, { withShovel }, noHarvestTier);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"soul_sand.png"), std::wstring(L"soul_sand"), stepSoulSand, stepSoulSand, stepSoulSand, breakSoulSand, breakSoulSand, lightBlocking, { withShovel }, noHarvestTier);
	identifier++;
	const wstringContainer sandTypeNames = {
		std::wstring(L"sand"),
		std::wstring(L"red_sand") };
	for (size_t i = 0; i < sandTypeNames.size(); i++)
	{
		blockList[identifier] = new block((blockID)identifier, 0.5, 0.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (sandTypeNames[i] + std::wstring(L".png"))), sandTypeNames[i], stepSand, stepSand, stepSand, digSand, digSand, sunlightPermeable, { withShovel }, noHarvestTier);
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 0.6, 0.6, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"gravel.png"), std::wstring(L"gravel"), stepGravel, stepGravel, stepGravel, digGravel, digGravel, sunlightPermeable, { withShovel }, noHarvestTier);
	identifier++;
	// ores

	cint stoneTypeCount = 0x3;
	std::wstring stoneTypeNames[stoneTypeCount]{ std::wstring(L"andesite"), std::wstring(L"diorite"), std::wstring(L"granite") };

	for (int i = 0; i < stoneTypeCount; i++)
	{

		// stone types
		blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (stoneTypeNames[i] + std::wstring(L".png"))), stoneTypeNames[i], stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
		identifier++;
		//'ite' blocks
		const std::wstring polishedName = std::wstring(L"polished_") + stoneTypeNames[i];
		blockList[identifier] = new block((blockID)identifier, 1.5, 1.5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (polishedName + std::wstring(L".png"))), polishedName, stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withPickaxe }, woodHarvestTier);
		identifier++;
	}

	for (int i = 0; i < woodTypeCount; i++)
	{
		if (i < normalTreeTypeCount)
		{
			// add sapling for each tree type
			blockList[identifier] = new block((blockID)identifier, 0.0, 0.0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_sapling.png"))), woodTypeDataList[i]->name + std::wstring(L"_sapling"), stepGrass, stepGrass, stepGrass, digGrass, digGrass, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide, 60, 100, true);
			identifier++;
		}
		else
		{
			// add fungus for each huge fungus type
			blockList[identifier] = new block((blockID)identifier, 0.0, 0.0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_fungus.png"))), woodTypeDataList[i]->name + std::wstring(L"_fungus"), breakFungus, breakFungus, breakFungus, breakFungus, breakFungus, noLightFilter, { withHand }, noHarvestTier, collisionTypeID::willNotCollide);
			identifier++;
		}
	}
	// collisionTypeID::willCollide so you can close the door before you and zombies can'T hit you
	blockList[identifier] = new block((blockID)identifier, 5.0, 5.0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"iron_trapdoor.png"), std::wstring(L"iron_trapdoor"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollide);
	identifier++;
	for (int i = 0; i < woodTypeCount; i++)
	{
		blockList[identifier] = new block((blockID)identifier, 3.0, 3.0, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_trapdoor.png"))), woodTypeDataList[i]->name + std::wstring(L"_trapdoor"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollide);
		identifier++;
	}

	// collisionTypeID::willCollide so you can close the door before you and zombies can'T hit you
	blockList[identifier] = new doubleBlock(block((blockID)identifier, 5.0, 5, standardBlockWeightPerCubicMeter,
		loadTextureFromResourcePack(itemTextureFolder / L"iron_door.png"), std::wstring(L"iron_door"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollide),
		loadTextureFromResourcePack(blockTextureFolder / L"iron_door_bottom.png"), loadTextureFromResourcePack(blockTextureFolder / L"iron_door_top.png"), veci2(0, 1));
	identifier++;
	for (int i = 0; i < woodTypeCount; i++)
	{
		blockList[identifier] = new doubleBlock(block((blockID)identifier, 3.0, 3, standardBlockWeightPerCubicMeter,
			loadTextureFromResourcePack(itemTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_door.png"))), woodTypeDataList[i]->name + std::wstring(L"_door"), stepWood, stepWood, stepWood, digWood, digWood,
			lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollide),
			loadTextureFromResourcePack(blockTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_door_bottom.png"))), loadTextureFromResourcePack(blockTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_door_top.png"))), veci2(0, 1));
		identifier++;
	}

	blockList[identifier] = new block((blockID)identifier, 5, 5, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"iron_bars.png"), std::wstring(L"iron_bars"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	// to get the planks textures
	blockList.update();

	for (int i = 0; i < woodTypeCount; i++)
	{
		resolutionTexture* tex = blockList[getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->tex;

		blockList[identifier] = new block((blockID)identifier, 2, 3, standardBlockWeightPerCubicMeter, tex, woodTypeDataList[i]->name + std::wstring(L"_fence_gate"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollide, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount);
		identifier++;
	}

	for (int i = 0; i < woodTypeCount; i++)
	{
		resolutionTexture* tex = blockList[getTreeBlock((woodTypeID)i, treeItemTypeID::planks)]->tex;

		blockList[identifier] = new block((blockID)identifier, 2, 3, standardBlockWeightPerCubicMeter, tex, woodTypeDataList[i]->name + std::wstring(L"_fence"), stepWood, stepWood, stepWood, digWood, digWood, lightFiltering, { withAxe }, noHarvestTier, collisionTypeID::willCollideTop, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount ? 5 : 0, i < normalTreeTypeCount);
		identifier++;
	}

	resolutionTexture* netherBrickFenceTexture = loadTextureFromResourcePack(blockTextureFolder / L"nether_bricks.png");

	blockList[identifier] = new block((blockID)identifier, 2, 6, standardBlockWeightPerCubicMeter, netherBrickFenceTexture, std::wstring(L"nether_brick_fence"), stepNetherBrick, stepNetherBrick, stepNetherBrick, digNetherBrick, digNetherBrick, lightFiltering, { withPickaxe }, woodHarvestTier, collisionTypeID::willCollideTop);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, -1, -1, standardBlockWeightPerCubicMeter, loadDragonEggTexture(), std::wstring(L"dragon_egg"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop, 0, 0, false, false, glowingLightSource);
	identifier++;

	blockList[identifier] = new block((blockID)identifier, -1, -1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"jigsaw_side.png"), std::wstring(L"jigsaw"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"structure_block.png"), std::wstring(L"structure_block"), stepStone, stepStone, stepStone, digStone, digStone, lightFiltering, { withPickaxe }, noHarvestTier, collisionTypeID::willCollideTop);
	identifier++;
	blockList[identifier] = new block((blockID)identifier, -1, -1, standardBlockWeightPerCubicMeter, loadTextureFromResourcePack(blockTextureFolder / L"bedrock.png"), std::wstring(L"bedrock"), stepStone, stepStone, stepStone, digStone, digStone, lightBlocking, { withHand }, noHarvestTier, collisionTypeID::willCollide);
	identifier++;

	blockList.update();
	loadBlockPowerProperties();
}

static void loadItems()
{
	// calculate possible enchantments, to use for loading the items
	const std::vector<enchantmentID>& normalEnchantments = { enchantmentID::unBreaking, enchantmentID::mending, enchantmentID::curseOfVanishing };
	std::vector<enchantmentID> armorEnchantments = { enchantmentID::protection, enchantmentID::fireProtection, enchantmentID::blastProtection, enchantmentID::projectileProtection, enchantmentID::thorns, enchantmentID::curseOfBinding };
	armorEnchantments.insert(armorEnchantments.end(), normalEnchantments.begin(), normalEnchantments.end());
	std::vector<enchantmentID> helmetEnchantments = { enchantmentID::aquaAffinity, enchantmentID::respiration };
	helmetEnchantments.insert(helmetEnchantments.end(), armorEnchantments.begin(), armorEnchantments.end());
	std::vector<enchantmentID> bootsEnchantments = { enchantmentID::depthStrider, enchantmentID::frostWalker, enchantmentID::soulSpeed, enchantmentID::featherFalling };
	bootsEnchantments.insert(bootsEnchantments.end(), armorEnchantments.begin(), armorEnchantments.end());
	std::vector<enchantmentID> swordEnchantments = { enchantmentID::sharpness, enchantmentID::baneOfArthropods, enchantmentID::smite, enchantmentID::knockback, enchantmentID::fireAspect, enchantmentID::looting, enchantmentID::sweepingEdge };
	swordEnchantments.insert(swordEnchantments.end(), normalEnchantments.begin(), normalEnchantments.end());
	std::vector<enchantmentID> axeEnchantments = { enchantmentID::sharpness, enchantmentID::baneOfArthropods, enchantmentID::smite };
	axeEnchantments.insert(axeEnchantments.end(), normalEnchantments.begin(), normalEnchantments.end());
	std::vector<enchantmentID> toolEnchantments = { enchantmentID::efficiency, enchantmentID::fortune, enchantmentID::silkTouch };
	toolEnchantments.insert(toolEnchantments.end(), normalEnchantments.begin(), normalEnchantments.end());
	std::vector<enchantmentID> shearsEnchantments = { enchantmentID::efficiency, enchantmentID::mending };
	shearsEnchantments.insert(shearsEnchantments.end(), normalEnchantments.begin(), normalEnchantments.end());

	std::vector<enchantmentID> bookEnchantments = {};
	for (int i = 0; i < (int)enchantmentID::count; i++)
	{
		bookEnchantments.push_back((enchantmentID)i);
	}

	// load items
	itemList = idList<itemData*, itemID>(fastList<itemData*>((int)itemID::count));

	// reset identifier to add the blocks as items
	int identifier = 0;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"air"), nullptr);
	identifier++;
	for (int i = 1; i < blockIDCount; i++)
	{
		itemList[identifier] = new itemData((itemID)i, blockList[i]->name, blockList[i]->tex);
		identifier++;
	}

	//'useless' items:items with no direct use
	// only for crafting
	for (int i = 0; i < (int)colorID::count; i++)
	{
		itemList[identifier] = new itemData((itemID)identifier, colorNames[i] + std::wstring(L"_dye"), loadTextureFromResourcePack(itemTextureFolder / (colorNames[i] + std::wstring(L"_dye.png"))));
		identifier++;
	}

	for (int i = 0; i < musicDiscTypeCount; i++)
	{
		const std::wstring& name = std::wstring(L"music_disc_") + musicDiscNames[i];
		itemList[identifier] = new itemData((itemID)identifier, name, loadTextureFromResourcePack(itemTextureFolder / (name + std::wstring(L".png"))), noHarvestTier, withHand, 1, 1, 1);
		identifier++;
	}


	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"stick"), loadTextureFromResourcePack(itemTextureFolder / L"stick.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"charcoal"), loadTextureFromResourcePack(itemTextureFolder / L"charcoal.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"clay_ball"), loadTextureFromResourcePack(itemTextureFolder / L"clay_ball.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"leather"), loadTextureFromResourcePack(itemTextureFolder / L"leather.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"paper"), loadTextureFromResourcePack(itemTextureFolder / L"paper.png"));
	identifier++;

	constexpr int bookEnchantability = 10;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"book"), loadTextureFromResourcePack(itemTextureFolder / L"book.png"), noHarvestTier, withHand, 1, 1, 64, false, noArmorTier, noArmorType, bookEnchantments, bookEnchantability);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"enchanted_book"), loadTextureFromResourcePack(itemTextureFolder / L"enchanted_book.png"), noHarvestTier, withHand, 1, 1, 1, false, noArmorTier, noArmorType, bookEnchantments, bookEnchantability);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"blaze_rod"), loadTextureFromResourcePack(itemTextureFolder / L"blaze_rod.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"blaze_powder"), loadTextureFromResourcePack(itemTextureFolder / L"blaze_powder.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"gunpowder"), loadTextureFromResourcePack(itemTextureFolder / L"gunpowder.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"ghast_tear"), loadTextureFromResourcePack(itemTextureFolder / L"ghast_tear.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"magma_cream"), loadTextureFromResourcePack(itemTextureFolder / L"magma_cream.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"flint"), loadTextureFromResourcePack(itemTextureFolder / L"flint.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"feather"), loadTextureFromResourcePack(itemTextureFolder / L"feather.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"string"), loadTextureFromResourcePack(itemTextureFolder / L"string.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"arrow"), loadTextureFromResourcePack(itemTextureFolder / L"arrow.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"brick"), loadTextureFromResourcePack(itemTextureFolder / L"brick.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"nether_brick"), loadTextureFromResourcePack(itemTextureFolder / L"nether_brick.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"clock"), loadTextureFromResourcePack(itemTextureFolder / L"clock_00.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"fermented_spider_eye"), loadTextureFromResourcePack(itemTextureFolder / L"fermented_spider_eye.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"glowstone_dust"), loadTextureFromResourcePack(itemTextureFolder / L"glowstone_dust.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"heart_of_the_sea"), loadTextureFromResourcePack(itemTextureFolder / L"heart_of_the_sea.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"honeycomb"), loadTextureFromResourcePack(itemTextureFolder / L"honeycomb.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"ink_sac"), loadTextureFromResourcePack(itemTextureFolder / L"ink_sac.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"nautilus_shell"), loadTextureFromResourcePack(itemTextureFolder / L"nautilus_shell.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"nether_star"), loadTextureFromResourcePack(itemTextureFolder / L"nether_star.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"netherite_ingot"), loadTextureFromResourcePack(itemTextureFolder / L"netherite_ingot.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"netherite_scrap"), loadTextureFromResourcePack(itemTextureFolder / L"netherite_scrap.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"phantom_membrane"), loadTextureFromResourcePack(itemTextureFolder / L"phantom_membrane.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"popped_chorus_fruit"), loadTextureFromResourcePack(itemTextureFolder / L"popped_chorus_fruit.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"prismarine_crystals"), loadTextureFromResourcePack(itemTextureFolder / L"prismarine_crystals.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"prismarine_shard"), loadTextureFromResourcePack(itemTextureFolder / L"prismarine_shard.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"rabbit_hide"), loadTextureFromResourcePack(itemTextureFolder / L"rabbit_hide.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"rabbit_foot"), loadTextureFromResourcePack(itemTextureFolder / L"rabbit_foot.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"scute"), loadTextureFromResourcePack(itemTextureFolder / L"scute.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"shulker_shell"), loadTextureFromResourcePack(itemTextureFolder / L"shulker_shell.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"slime_ball"), loadTextureFromResourcePack(itemTextureFolder / L"slime_ball.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"firework_star"), loadTextureFromResourcePack(itemTextureFolder / L"firework_star.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"firework_rocket"), loadTextureFromResourcePack(itemTextureFolder / L"firework_rocket.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"glass_bottle"), loadTextureFromResourcePack(itemTextureFolder / L"glass_bottle.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"honey_bottle"), loadTextureFromResourcePack(itemTextureFolder / L"honey_bottle.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"experience_bottle"), loadTextureFromResourcePack(itemTextureFolder / L"experience_bottle.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"dragon_breath"), loadTextureFromResourcePack(itemTextureFolder / L"dragon_breath.png"), noHarvestTier, withHand, 1, 1, 1);
	identifier++;

	auto potionTexture = loadTextureFromResourcePack(itemTextureFolder / L"potion.png");
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"awkward_potion"), potionTexture, noHarvestTier, withHand, 1, 1, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"mundane_potion"), potionTexture, noHarvestTier, withHand, 1, 1, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"thick_potion"), potionTexture, noHarvestTier, withHand, 1, 1, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"potion"), potionTexture, noHarvestTier, withHand, 1, 1, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"splash_potion"), loadTextureFromResourcePack(itemTextureFolder / L"splash_potion.png"), noHarvestTier, withHand, 1, 1, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"lingering_potion"), loadTextureFromResourcePack(itemTextureFolder / L"lingering_potion.png"), noHarvestTier, withHand, 1, 1, 1);
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"bow"), loadTextureFromResourcePack(itemTextureFolder / L"bow.png"), noHarvestTier, withHand, 1.0, 1.0, 1, true);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"egg"), loadTextureFromResourcePack(itemTextureFolder / L"egg.png"), noHarvestTier, withHand, 1.0, 1.0, 0x10);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"fishing_rod"), loadTextureFromResourcePack(itemTextureFolder / L"fishing_rod.png"), noHarvestTier, withHand, 1.0, 1.0, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"snowball"), loadTextureFromResourcePack(itemTextureFolder / L"snowball.png"), noHarvestTier, withHand, 1.0, 1.0, 0x10);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"ender_pearl"), loadTextureFromResourcePack(itemTextureFolder / L"ender_pearl.png"), noHarvestTier, withHand, 1.0, 1.0, 0x10);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"ender_eye"), loadTextureFromResourcePack(itemTextureFolder / L"ender_eye.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"end_crystal"), loadTextureFromResourcePack(itemTextureFolder / L"end_crystal.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"wheat_seeds"), loadTextureFromResourcePack(itemTextureFolder / L"wheat_seeds.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"carrot"), loadTextureFromResourcePack(itemTextureFolder / L"carrot.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"potato"), loadTextureFromResourcePack(itemTextureFolder / L"potato.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"beetroot_seeds"), loadTextureFromResourcePack(itemTextureFolder / L"beetroot_seeds.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"melon_seeds"), loadTextureFromResourcePack(itemTextureFolder / L"melon_seeds.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"pumpkin_seeds"), loadTextureFromResourcePack(itemTextureFolder / L"pumpkin_seeds.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"glistering_melon_slice"), loadTextureFromResourcePack(itemTextureFolder / L"glistering_melon_slice.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"melon_slice"), loadTextureFromResourcePack(itemTextureFolder / L"melon_slice.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"poisonous_potato"), loadTextureFromResourcePack(itemTextureFolder / L"poisonous_potato.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"sweet_berries"), loadTextureFromResourcePack(itemTextureFolder / L"sweet_berries.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"chorus_fruit"), loadTextureFromResourcePack(itemTextureFolder / L"chorus_fruit.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"dried_kelp"), loadTextureFromResourcePack(itemTextureFolder / L"dried_kelp.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"apple"), loadTextureFromResourcePack(itemTextureFolder / L"apple.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"beetroot_soup"), loadTextureFromResourcePack(itemTextureFolder / L"beetroot_soup.png"), noHarvestTier, withHand, 0.0, INFINITY, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"baked_potato"), loadTextureFromResourcePack(itemTextureFolder / L"baked_potato.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"rotten_flesh"), loadTextureFromResourcePack(itemTextureFolder / L"rotten_flesh.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"golden_carrot"), loadTextureFromResourcePack(itemTextureFolder / L"golden_carrot.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"porkchop"), loadTextureFromResourcePack(itemTextureFolder / L"porkchop.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_porkchop"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_porkchop.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"beef"), loadTextureFromResourcePack(itemTextureFolder / L"beef.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_beef"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_beef.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"mutton"), loadTextureFromResourcePack(itemTextureFolder / L"mutton.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_mutton"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_mutton.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"chicken"), loadTextureFromResourcePack(itemTextureFolder / L"chicken.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_chicken"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_chicken.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"rabbit"), loadTextureFromResourcePack(itemTextureFolder / L"rabbit.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_rabbit"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_rabbit.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cod"), loadTextureFromResourcePack(itemTextureFolder / L"cod.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_cod"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_cod.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"salmon"), loadTextureFromResourcePack(itemTextureFolder / L"salmon.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"cooked_salmon"), loadTextureFromResourcePack(itemTextureFolder / L"cooked_salmon.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"tropical_fish"), loadTextureFromResourcePack(itemTextureFolder / L"tropical_fish.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"pufferfish"), loadTextureFromResourcePack(itemTextureFolder / L"pufferfish.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"spider_eye"), loadTextureFromResourcePack(itemTextureFolder / L"spider_eye.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"bread"), loadTextureFromResourcePack(itemTextureFolder / L"bread.png"));
	identifier++;

	resolutionTexture* goldenAppleTexture = loadTextureFromResourcePack(itemTextureFolder / L"golden_apple.png");

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"golden_apple"), goldenAppleTexture);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"enchanted_golden_apple"), goldenAppleTexture);
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"mushroom_stew"), loadTextureFromResourcePack(itemTextureFolder / L"mushroom_stew.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"rabbit_stew"), loadTextureFromResourcePack(itemTextureFolder / L"rabbit_stew.png"));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"bowl"), loadTextureFromResourcePack(itemTextureFolder / L"bowl.png"), noHarvestTier, withHand, 1.0, 1.0, 0x10);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"bucket"), loadTextureFromResourcePack(itemTextureFolder / L"bucket.png"), noHarvestTier, withHand, 1.0, 1.0, 0x10);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"water_bucket"), loadTextureFromResourcePack(itemTextureFolder / L"water_bucket.png"), noHarvestTier, withHand, 1.0, 1.0, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"lava_bucket"), loadTextureFromResourcePack(itemTextureFolder / L"lava_bucket.png"), noHarvestTier, withHand, 1.0, 1.0, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"milk_bucket"), loadTextureFromResourcePack(itemTextureFolder / L"milk_bucket.png"), noHarvestTier, withHand, 1.0, 1.0, 1);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"flint_and_steel"), loadTextureFromResourcePack(itemTextureFolder / L"flint_and_steel.png"), noHarvestTier, withHand, 1.0, 1.0, 1, true, noArmorTier, noArmorType, std::vector<enchantmentID>(normalEnchantments));
	identifier++;

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"bone"), loadTextureFromResourcePack(itemTextureFolder / L"bone.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"bone_meal"), loadTextureFromResourcePack(itemTextureFolder / L"bone_meal.png"));
	identifier++;

	for (int i = 0; i < oreBlockTypeCount - 2; i++)
	{
		std::wstring oreItemName = oreBlockNames[i];
		if (smeltable[i])
		{
			oreItemName = L"raw_" + oreItemName;
		}
		if (oreItemName == std::wstring(L"lapis"))
		{
			oreItemName += std::wstring(L"_lazuli");
		}
		itemList[identifier] = new itemData((itemID)identifier, oreItemName, loadTextureFromResourcePack(itemTextureFolder / (oreItemName + std::wstring(L".png"))));
		identifier++;
	}
	itemList[identifier] = new itemData((itemID)identifier, L"quartz", loadTextureFromResourcePack(itemTextureFolder / L"quartz.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, L"gold_ingot", loadTextureFromResourcePack(itemTextureFolder / L"gold_ingot.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, L"iron_ingot", loadTextureFromResourcePack(itemTextureFolder / L"iron_ingot.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, L"gold_nugget", loadTextureFromResourcePack(itemTextureFolder / L"gold_nugget.png"));
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, L"iron_nugget", loadTextureFromResourcePack(itemTextureFolder / L"iron_nugget.png"));
	identifier++;

	// spawn eggs will be added after the entity list is added
	identifier += mobTypeCount;

	const wstringContainer minecartNames = { std::wstring(L"minecart"), std::wstring(L"chest_minecart"), std::wstring(L"furnace_minecart"), std::wstring(L"hopper_minecart"), std::wstring(L"tnt_minecart"), std::wstring(L"command_block_minecart") };

	for (int i = 0; i < mineCartItemTypeCount; i++)
	{
		itemList[identifier] = new itemData((itemID)identifier, minecartNames[i], loadTextureFromResourcePack(itemTextureFolder / (minecartNames[i] + std::wstring(L".png"))), noHarvestTier, withHand, 1, 1, 1);
		identifier++;
	}

	for (int i = 0; i < normalTreeTypeCount; i++)
	{
		itemList[identifier] = new itemData((itemID)identifier, woodTypeDataList[i]->name + std::wstring(L"_boat"), loadTextureFromResourcePack(itemTextureFolder / (woodTypeDataList[i]->name + std::wstring(L"_boat.png"))), noHarvestTier, withHand, 1.0, 1.0, 1);
		identifier++;
		boatTextures[i] = loadTextureFromResourcePack(entityTextureFolder / L"boat" / (woodTypeDataList[i]->name + std::wstring(L".png")));
	}

	std::vector<enchantmentID> armorTypeEnchantments[armorTypeCount]{
		bootsEnchantments,
		armorEnchantments,
		armorEnchantments,
		helmetEnchantments };

	cint toolTypeCount = 6; // sword, pickaxe, axe, shovel, hoe, shears
	std::vector<enchantmentID> toolTypeEnchantments[toolTypeCount]{
		swordEnchantments,
		toolEnchantments,
		axeEnchantments,
		toolEnchantments,
		toolEnchantments,
		toolEnchantments };

	// add tools
	wstringContainer toolTierNames = { std::wstring(L"wooden_"), std::wstring(L"golden_"), std::wstring(L"stone_"), std::wstring(L"iron_"), std::wstring(L"diamond_"), std::wstring(L"netherite_") };
	wstringContainer toolTypeNames = { std::wstring(L"sword"), std::wstring(L"pickaxe"), std::wstring(L"axe"), std::wstring(L"shovel"), std::wstring(L"hoe") };
	std::vector<fp> attackSpeeds = { 1.6, 1.2, 0.8, 1, 0 };
	std::vector<fp> hoeAttackSpeeds = { 1, 1, 2, 3, 4, 4 };
	std::vector<fp> axeAttackSpeeds = { 0.8, 1, 0.8, 0.9, 1, 1 };
	std::vector<fp> baseAttackDamageList = { 4, 2, 0, 2.5, 0 };
	std::vector<fp> axeAttackDamageList = { 7, 7, 9, 9, 9, 10 };
	std::vector<fp> tierAttackDamageList = { 0, 0, 1, 2, 3, 4 };
	for (int toolTierIndex = 0; toolTierIndex < toolTierNames.size(); toolTierIndex++)
	{
		std::wstring toolTierName = toolTierNames[toolTierIndex];
		for (int toolTypeIndex = 0; toolTypeIndex < toolTypeNames.size(); toolTypeIndex++)
		{
			std::wstring toolTypeName = toolTypeNames[toolTypeIndex];
			cfp attackSpeed = toolTypeIndex == (withHoe - 1) ? hoeAttackSpeeds[toolTierIndex] : toolTypeIndex == (withAxe - 1) ? axeAttackSpeeds[toolTierIndex]
				: attackSpeeds[toolTypeIndex];
			cfp attackDamage = toolTypeIndex == (withHoe - 1) ? 1 : toolTypeIndex == (withAxe - 1) ? axeAttackDamageList[toolTierIndex]
				: baseAttackDamageList[toolTypeIndex] + tierAttackDamageList[toolTierIndex];
			std::wstring toolName = toolTierName + toolTypeName;
			itemList[identifier] = new itemData((itemID)identifier, toolName, loadTextureFromResourcePack(itemTextureFolder / (toolName + std::wstring(L".png"))), (harvestTierID)(toolTierIndex + 1), harvestTypeID(toolTypeIndex + 1), attackDamage, attackSpeed, 1, true, noArmorTier, noArmorType, std::vector<enchantmentID>(toolTypeEnchantments[toolTypeIndex]), toolEnchantability[toolTierIndex]);
			identifier++;
		}
	}

	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"shears"), loadTextureFromResourcePack(itemTextureFolder / L"shears.png"), ironHarvestTier, withSwordOrShears, 1.0, 1.0, 1, true, noArmorTier, noArmorType, std::vector<enchantmentID>(shearsEnchantments), 14);
	identifier++;
	itemList[identifier] = new itemData((itemID)identifier, std::wstring(L"elytra"), loadTextureFromResourcePack(itemTextureFolder / L"elytra.png"), noHarvestTier, withHand, 1.0, 1.0, 1, true, noArmorTier, chestplateArmorType, std::vector<enchantmentID>(normalEnchantments), 15);
	identifier++;

	// add armor
	wstringContainer armorTierNames = { std::wstring(L"turtle_"), std::wstring(L"leather_"), std::wstring(L"golden_"), std::wstring(L"chainmail_"), std::wstring(L"iron_"), std::wstring(L"diamond_"), std::wstring(L"netherite_") };
	wstringContainer armorTypeNames = { std::wstring(L"boots"), std::wstring(L"leggings"), std::wstring(L"chestplate"), std::wstring(L"helmet") };
	wstringContainer armorMaterialNames = { std::wstring(L"turtle"), std::wstring(L"leather"), std::wstring(L"gold"), std::wstring(L"chainmail"), std::wstring(L"iron"), std::wstring(L"diamond"), std::wstring(L"netherite") };
	wstringContainer armorSoundNames = { std::wstring(L"(no sound)"), std::wstring(L"leather"), std::wstring(L"gold"), std::wstring(L"chain"), std::wstring(L"iron"), std::wstring(L"diamond"), std::wstring(L"netherite") };

	const color leatherColor = hexToColor(0xA06540);

	for (int armorTierIndex = 0; armorTierIndex < armorTierNames.size(); armorTierIndex++)
	{
		std::wstring armorTierName = armorTierNames[armorTierIndex];
		for (int armorTypeIndex = 0; armorTypeIndex < armorTypeNames.size(); armorTypeIndex++)
		{
			if (armorTierIndex > 0 || armorTypeIndex == helmetArmorType - bootsArmorType)
			{
				std::wstring armorTypeName = armorTypeNames[armorTypeIndex];

				std::wstring armorName = armorTierName + armorTypeName;
				resolutionTexture* itemTexture = loadTextureFromResourcePack(itemTextureFolder / (armorName + std::wstring(L".png")));
				if (armorTierIndex == leatherArmorTier - turtleArmorTier) // leather
				{
					// multiply colors
					multiplyRectangle(itemTexture->getClientRect(), leatherColor, *itemTexture);
				}

				itemList[identifier] = new itemData((itemID)identifier, armorName, itemTexture, noHarvestTier, withHand, 1, 1, 1, false, (armorTierID)(armorTierIndex + turtleArmorTier), (armorTypeID)(armorTypeIndex + 1), std::vector<enchantmentID>(armorTypeEnchantments[armorTypeIndex]), armorEnchantability[armorTierIndex]);
				identifier++;
			}
		}
		armorTextures[armorTierIndex] = loadTextureFromResourcePack(armorTextureFolder / (armorMaterialNames[armorTierIndex] + std::wstring(L"_layer_1.png")));
		armorLegTextures[armorTierIndex] = armorTierIndex > 0 ? loadTextureFromResourcePack(armorTextureFolder / (armorMaterialNames[armorTierIndex] + std::wstring(L"_layer_2.png"))) : nullptr;
		armorEquipSound[armorTierIndex] = armorTierIndex ? std::make_shared<audioCollection>(itemSoundFolder / L"armor" / (L"equip_" + armorSoundNames[armorTierIndex]), AudioType::sound) : equipSound;
	}

	resolutionTexture* leatherArmorTexture = armorTextures[leatherArmorTier - 1];
	multiplyRectangle(leatherArmorTexture->getClientRect(), leatherColor, *leatherArmorTexture);
	leatherArmorTexture = armorLegTextures[leatherArmorTier - 1];
	multiplyRectangle(leatherArmorTexture->getClientRect(), leatherColor, *leatherArmorTexture);

	// dont add any items after this
	itemList.update();
}

static void loadEntityData()
{
	// add mob data
	entityDataList = idList<entityData*, entityID>(fastList<entityData*>());
	int currentEntityID = 0;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"human"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTexture(skinFolder / L"current.png", humanSkinSize), nullptr, nullptr, std::make_shared<audioCollection>(generalSoundFolder / L"damage" / L"hit", AudioType::sound), nullptr, humanWalkingSpeed, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::pink], rgbColorValues[(int)colorID::green], 4.0, 1.0, new experienceDrop(), humanFlyingSpeed));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"skeleton"), maxhumanhealth, humanHitbox, skeletonVolume, skeletonWeight), loadTextureFromResourcePack(entityTextureFolder / L"skeleton" / L"skeleton.png"), std::make_shared<audioCollection>(mobSoundFolder / L"skeleton" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"skeleton" / L"say", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"skeleton" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"skeleton" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::lightGray], rgbColorValues[(int)colorID::gray], 1.5, 5, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"stray"), maxhumanhealth, humanHitbox, skeletonVolume, skeletonWeight), loadTextureFromResourcePack(entityTextureFolder / L"skeleton" / L"stray.png"), std::make_shared<audioCollection>(mobSoundFolder / L"stray" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"stray" / L"idle", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"stray" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"stray" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::blue], rgbColorValues[(int)colorID::gray], 1.5, 5, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"bogged"), maxhumanhealth, humanHitbox, skeletonVolume, skeletonWeight), loadTextureFromResourcePack(entityTextureFolder / L"skeleton" / L"bogged.png"), std::make_shared<audioCollection>(mobSoundFolder / L"skeleton" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"bogged" / L"ambient", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"bogged" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"bogged" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::lime], rgbColorValues[(int)colorID::green], 1.5, 5, new experienceDrop(5, 5)));
	currentEntityID++;

	std::shared_ptr<audioCollection> zombieStep = std::make_shared<audioCollection>(mobSoundFolder / L"zombie" / L"step", AudioType::sound);

	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"husk"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTextureFromResourcePack(entityTextureFolder / L"zombie" / L"husk.png"), std::make_shared<audioCollection>(mobSoundFolder / L"husk" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"husk" / L"idle", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"husk" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"husk" / L"death", AudioType::sound), 1.0, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::lightBlue], rgbColorValues[(int)colorID::green], 1.5, 3, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"drowned"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTextureFromResourcePack(entityTextureFolder / L"zombie" / L"drowned.png"), std::make_shared<audioCollection>(mobSoundFolder / L"drowned" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"drowned" / L"idle", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"drowned" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"drowned" / L"death", AudioType::sound), 1.0, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::lightBlue], rgbColorValues[(int)colorID::green], 1.5, 3, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"zombie_villager"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTextureFromResourcePack(entityTextureFolder / L"zombie" / L"zombie_villager.png"), zombieStep, std::make_shared<audioCollection>(mobSoundFolder / L"zombie_villager" / L"say", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"zombie_villager" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"zombie_villager" / L"death", AudioType::sound), 1.0, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::lightBlue], rgbColorValues[(int)colorID::green], 1.5, 3, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"zombie"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTextureFromResourcePack(entityTextureFolder / L"zombie" / L"zombie.png"), zombieStep, std::make_shared<audioCollection>(mobSoundFolder / L"zombie" / L"say", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"zombie" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"zombie" / L"death", AudioType::sound), 1.0, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::lightBlue], rgbColorValues[(int)colorID::green], 1.5, 3, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"zombified_piglin"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTextureFromResourcePack(entityTextureFolder / L"piglin" / L"zombified_piglin.png"), zombieStep, std::make_shared<audioCollection>(mobSoundFolder / L"zombified_piglin" / L"zpig", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"zombified_piglin" / L"zpighurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"zombified_piglin" / L"zpigdeath", AudioType::sound), 1.0, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::pink], rgbColorValues[(int)colorID::brown], 1.5, 3, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"villager"), maxhumanhealth, humanHitbox, humanVolume, humanWeight), loadTextureFromResourcePack(entityTextureFolder / L"villager" / L"villager.png"), nullptr, std::make_shared<audioCollection>(mobSoundFolder / L"villager" / L"idle", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"villager" / L"hit", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"villager" / L"death", AudioType::sound), 1.0, getLegSwingSynchronizer(humanLegSize.y), rgbColorValues[(int)colorID::brown], rgbColorValues[(int)colorID::pink]));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"enderman"), 40, endermanHitbox, 0.1, 100), loadTextureFromResourcePack(entityTextureFolder / L"enderman" / L"enderman.png"), nullptr, std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"idle", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"hit", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"endermen" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(endermanLegSize.y, maxEndermanLegAngle), rgbColorValues[(int)colorID::black], rgbColorValues[(int)colorID::purple], 1.5, 7, new experienceDrop(5, 5)));
	currentEntityID++;

	resolutionTexture* unEditedSlimeTexture = loadTextureFromResourcePack(entityTextureFolder / L"slime" / L"slime.png");

	auto editedSlimeTexture = new resolutionTexture(texture(vect2<fsize_t>(editedSlimeTextureRect.size), false), cvec2(editedSlimeTextureRect.size));

	fillTransparentRectangle(crectangle2(slimeInnerBodyTextureRect), crectangle2((editedSlimeTextureRect.size - slimeInnerBodyTextureRect.size) / 2, slimeInnerBodyTextureRect.size), *unEditedSlimeTexture, *editedSlimeTexture);
	fillTransparentRectangle(crectangle2(slimeEyeTextureRect), crectangle2(cvec2(0xb, 0x8), slimeEyeTextureRect.size), *unEditedSlimeTexture, *editedSlimeTexture);
	fillTransparentRectangle(crectangle2(slimeOuterBodyTextureRect), crectangle2(editedSlimeTextureRect), *unEditedSlimeTexture, *editedSlimeTexture);

	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"slime"), 0, crectangle2()), editedSlimeTexture, nullptr, nullptr, nullptr, nullptr, humanWalkingSpeed, getLegSwingSynchronizer(0, 0), rgbColorValues[(int)colorID::lime], rgbColorValues[(int)colorID::green], 2, 0, new experienceDrop(1, 2)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"magma_cube"), 0, crectangle2()), loadTextureFromResourcePack(entityTextureFolder / L"slime" / L"magmacube.png"), nullptr, nullptr, nullptr, nullptr, humanWalkingSpeed, getLegSwingSynchronizer(0, 0), rgbColorValues[(int)colorID::brown], rgbColorValues[(int)colorID::yellow], 2, 0, new experienceDrop(1, 3)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"creeper"), 20, creeperHitbox, humanVolume, 160), loadTextureFromResourcePack(entityTextureFolder / L"creeper" / L"creeper.png"), nullptr, nullptr, std::make_shared<audioCollection>(mobSoundFolder / L"creeper" / L"say", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"creeper" / L"death", AudioType::sound), 2, getLegSwingSynchronizer(creeperLegSize.y), rgbColorValues[(int)colorID::green], rgbColorValues[(int)colorID::black], 0, 0, new experienceDrop(5, 5)));
	currentEntityID++;
	std::shared_ptr<audioCollection> oink = std::make_shared<audioCollection>(mobSoundFolder / L"pig" / L"say", AudioType::sound);

	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"wolf"), 10, relativeWolfHitbox, 0.029, 30), loadTextureFromResourcePack(entityTextureFolder / L"wolf" / L"wolf.png"), std::make_shared<audioCollection>(mobSoundFolder / L"wolf" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"wolf" / L"bark", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"wolf" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"wolf" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(wolfLegSize.y), rgbColorValues[(int)colorID::lightGray], rgbColorValues[(int)colorID::brown], 0.5, 4, new experienceDrop(1, 3)));
	currentEntityID++;

	// weights of natural animals, not domesticated animals
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"pig"), 10, crectangle2(-pigHitboxSize.x * 0.5, 0, pigHitboxSize.x, pigHitboxSize.y), 0.25, 250), loadTextureFromResourcePack(entityTextureFolder / L"pig" / L"pig.png"), std::make_shared<audioCollection>(mobSoundFolder / L"pig" / L"step", AudioType::sound), oink, oink, std::make_shared<audioCollection>(mobSoundFolder / L"pig" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(pigLegSize.y), rgbColorValues[(int)colorID::pink], rgbColorValues[(int)colorID::red], 1.0, 1.0, new experienceDrop(1, 3)));
	currentEntityID++;
	std::shared_ptr<audioCollection> cowHurt = std::make_shared<audioCollection>(mobSoundFolder / L"cow" / L"hurt", AudioType::sound);
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"cow"), 10, crectangle2(-cowHitboxSize.x * 0.5, 0, cowHitboxSize.x, cowHitboxSize.y), 0.2, 200), loadTextureFromResourcePack(entityTextureFolder / L"cow" / L"cow.png"), std::make_shared<audioCollection>(mobSoundFolder / L"cow" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"cow" / L"say", AudioType::sound), cowHurt, cowHurt, humanWalkingSpeed, getLegSwingSynchronizer(cowLegSize.y), rgbColorValues[(int)colorID::brown], rgbColorValues[(int)colorID::gray], 1.0, 1.0, new experienceDrop(1, 3)));
	currentEntityID++;
	std::shared_ptr<audioCollection> baa = std::make_shared<audioCollection>(mobSoundFolder / L"sheep" / L"say", AudioType::sound);
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"sheep"), 8, crectangle2(-sheepHitboxSize.x * 0.5, 0, sheepHitboxSize.x, sheepHitboxSize.y), 0.07, 70), loadTextureFromResourcePack(entityTextureFolder / L"sheep" / L"sheep.png"), std::make_shared<audioCollection>(mobSoundFolder / L"sheep" / L"step", AudioType::sound), baa, baa, baa, humanWalkingSpeed, getLegSwingSynchronizer(sheepLegSize.y), rgbColorValues[(int)colorID::white], rgbColorValues[(int)colorID::pink], 1.0, 1.0, new experienceDrop(1, 3)));
	currentEntityID++;
	std::shared_ptr<audioCollection> chickenHurt = std::make_shared<audioCollection>(mobSoundFolder / L"chicken" / L"hurt", AudioType::sound);
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"chicken"), 4, crectangle2(-chickenHitboxSize.x * 0.5, 0, chickenHitboxSize.x, chickenHitboxSize.y), 0.01, 10), loadTextureFromResourcePack(entityTextureFolder / L"chicken.png"), std::make_shared<audioCollection>(mobSoundFolder / L"chicken" / L"step", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"chicken" / L"say", AudioType::sound), chickenHurt, chickenHurt, humanWalkingSpeed, getLegSwingSynchronizer(chickenUpperLegSize.y), rgbColorValues[(int)colorID::lightGray], rgbColorValues[(int)colorID::red], 1.0, 1.0, new experienceDrop(1, 3)));
	currentEntityID++;

	std::shared_ptr<audioCollection> fishHurt = std::make_shared<audioCollection>(entitySoundFolder / L"fish" / L"hurt", AudioType::sound);
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"cod"), 3, relativeCodHitbox, 0.015, 15), loadTextureFromResourcePack(entityTextureFolder / L"fish" / L"cod.png"), std::make_shared<audioCollection>(entitySoundFolder / L"fish" / L"flop", AudioType::sound), std::make_shared<audioCollection>(entitySoundFolder / L"fish" / L"swim", AudioType::sound), fishHurt, fishHurt, humanWalkingSpeed, getLegSwingSynchronizer(chickenUpperLegSize.y), rgbColorValues[(int)colorID::yellow], rgbColorValues[(int)colorID::brown], 1.0, 1.0, new experienceDrop(1, 3)));
	currentEntityID++;

	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"shulker"), 30, shulkerRelativeHitbox, 1, 50), loadTextureFromResourcePack(entityTextureFolder / L"shulker" / L"shulker.png"), nullptr, std::make_shared<audioCollection>(entitySoundFolder / L"shulker" / L"ambient", AudioType::sound), std::make_shared<audioCollection>(entitySoundFolder / L"shulker" / L"hurt", AudioType::sound), std::make_shared<audioCollection>(entitySoundFolder / L"shulker" / L"death", AudioType::sound), 0, getLegSwingSynchronizer(0), rgbColorValues[(int)colorID::purple], rgbColorValues[(int)colorID::black], 1.0, 1.0, new experienceDrop(5, 5)));
	currentEntityID++;

	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"blaze"), 20, blazeHitbox, 0.04, 300), loadTextureFromResourcePack(entityTextureFolder / L"blaze.png"), nullptr, std::make_shared<audioCollection>(mobSoundFolder / L"blaze" / L"breathe", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"blaze" / L"hit", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"blaze" / L"death", AudioType::sound), humanWalkingSpeed, getLegSwingSynchronizer(0, 0), rgbColorValues[(int)colorID::yellow], rgbColorValues[(int)colorID::green], 1.5, 6, new experienceDrop(10, 10)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"ghast"), 10, ghastHitbox, ghastHitboxSize.volume() * ghastHitboxSize.x, 150), loadTextureFromResourcePack(entityTextureFolder / L"ghast" / L"ghast.png"), nullptr, std::make_shared<audioCollection>(mobSoundFolder / L"ghast" / L"moan", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"ghast" / L"scream", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"ghast" / L"death", AudioType::sound), ghastFlyingSpeed, getLegSwingSynchronizer(0, 0), rgbColorValues[(int)colorID::white], rgbColorValues[(int)colorID::gray], 0, 0, new experienceDrop(5, 5)));
	currentEntityID++;
	entityDataList.push_back(new mobData(entityData((entityID)currentEntityID, std::wstring(L"ender_dragon"), 200, enderDragonHitbox, enderDragonVolume, enderDragonWeight), loadTextureFromResourcePack(entityTextureFolder / L"enderdragon" / L"dragon.png"), nullptr, std::make_shared<audioCollection>(mobSoundFolder / L"enderdragon" / L"growl", AudioType::sound), std::make_shared<audioCollection>(mobSoundFolder / L"enderdragon" / L"hit", AudioType::sound), nullptr, enderDragonFlyingSpeed, waveShaper(0, 0, 0), rgbColorValues[(int)colorID::purple], rgbColorValues[(int)colorID::blue], INFINITY, enderDragonHeadAttackDamage));
	currentEntityID++;
	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"boat"), 4, crectangle2(boatHitboxSize.x * -0.5, 0, boatHitboxSize.x, boatHitboxSize.y), math::squared(boatHitboxSize.x) * boatHitboxSize.y));
	currentEntityID++;

	resolutionTexture* unEditedMinecartTexture = loadTextureFromResourcePack(entityTextureFolder / L"minecart.png");
	auto minecartEditedTexture = new resolutionTexture(texture(vect2<fsize_t>(minecartEditedTextureRect.size)), minecartEditedTextureRect.size);
	fillTransformedBrushRectangle(minecartBottomTextureRect, cvec2(), *unEditedMinecartTexture, *minecartEditedTexture);
	fillTransformedBrushRectangle(minecartTopTextureRect, cvec2(0, minecartBottomTextureRect.size.y), *unEditedMinecartTexture, *minecartEditedTexture);
	minecartTexture = minecartEditedTexture;

	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"minecart"), 6, crectangle2(minecartHitboxSize.x * -0.5, 0, minecartHitboxSize.x, minecartHitboxSize.y), 0.2, 0));
	currentEntityID++;
	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"item"), 5, crectangle2(vec2(itemSize * -0.5, 0), vec2(itemSize, 0.9)), 1.0 / 8, 8));
	currentEntityID++;
	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"experience_orb"), 5, relativeExperienceHitbox, 0.01, 2));
	currentEntityID++;

	for (size_t i = 0; i < (size_t)particleID::count; i++)
	{
		entityDataList.push_back(new entityData((entityID)currentEntityID, particleTypeDataList[(particleID)i]->name + std::wstring(L"_particle"), 5, relativeParticleHitbox, 0.002, 0.1, false));
		currentEntityID++;
	}

	cfp arrowPower = 0x40;
	cfp throwPower = 0x30;
	cfp fishingHookThrowPower = 0xc;

	constexpr fp throwableVolume = 0.05 * 0.05 * 0.05;

	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"snowball"), INFINITY, crectangle2().expanded(0.25), throwableVolume, 5), itemList[itemID::snowball]->tex, throwPower));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"egg"), INFINITY, crectangle2().expanded(0.25), throwableVolume, 5), itemList[itemID::egg]->tex, throwPower));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"ender_pearl"), INFINITY, crectangle2().expanded(0.25), throwableVolume, 10), itemList[itemID::ender_pearl]->tex, throwPower));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"ender_eye"), INFINITY, crectangle2().expanded(0.25), throwableVolume, 5), itemList[itemID::ender_eye]->tex, 0));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"fishing_hook"), INFINITY, crectangle2().expanded(0.25), throwableVolume, 5), loadTextureFromResourcePack(entityTextureFolder / L"fishing_hook.png"), fishingHookThrowPower));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"arrow"), INFINITY, relativeArrowHitbox, throwableVolume, 5), loadTextureFromResourcePack(entityTextureFolder / L"projectiles" / L"arrow.png"), arrowPower));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"fireball"), INFINITY, crectangle2().expanded(0.5), throwableVolume, 10), fireChargeTexture, 0));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"dragon_fireball"), INFINITY, crectangle2().expanded(0.5), throwableVolume, 20), loadTextureFromResourcePack(entityTextureFolder / L"enderdragon" / L"dragon_fireball.png"), 0));
	currentEntityID++;
	entityDataList.push_back(new throwableData(entityData((entityID)currentEntityID, std::wstring(L"small_fireball"), INFINITY, crectangle2().expanded(5.0 / 0x20), throwableVolume, 5), fireChargeTexture, 0));
	currentEntityID++;

	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"tnt"), INFINITY, crectangle2().expanded(0.5), 1, 200));
	currentEntityID++;
	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"end_crystal"), 5, endCrystalRelativeHitbox, 1.0 / 8, 500));
	currentEntityID++;

	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"falling_block"), INFINITY, crectangle2(-0.5, 0, 1, 1), 1, 50));
	currentEntityID++;
	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"piston"), INFINITY, crectangle2()));
	currentEntityID++;
	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"firework_rocket"), INFINITY, crectangle2(-0.125, -0.125, 0.25, 0.25), 0.1, 5));
	currentEntityID++;

	entityDataList.push_back(new entityData((entityID)currentEntityID, std::wstring(L"pollen"), 5, relativePollenHitbox, 0.01 * 0.01 * 0.01, 0.001));
	currentEntityID++;

	entityDataList.update();
}

void loadResourcePacks()
{
	loadDataLists();
	loadMusic();
	// load textures

	// non-resource pack:
	// world gen
	biomeTexture = loadTextureFromResourcePack(overworldDataFolder / L"biomes.png");
	tempMapTexture = loadTextureFromResourcePack(overworldDataFolder / L"temperatures.png");

	experienceTexture = loadTextureFromResourcePack(entityTextureFolder / L"experience_orb.png");

	// gui
	currentMinecraftFontFamily = new fontFamily(loadTextureFromResourcePack(guiTextureFolder / L"ascii shadow.png"));
	currentMinecraftFont = new minecraftFont();

	mainMenuBackgroundTexture = loadTextureFromResourcePack(guiTextureFolder / L"title" / L"background" / L"2d.png");

	//barsTexture = loadTextureFromResourcePack(guiTextureFolder / L"bars.png");

	chatButtonTexture = loadTextureFromResourcePack(buttonTextureFolder / L"chat.png");
	settingsButtonTexture = loadTextureFromResourcePack(buttonTextureFolder / L"settings.png");
	inventoryButtonTexture = loadTextureFromResourcePack(buttonTextureFolder / L"inventory.png");

	grassOverlay = loadTextureFromResourcePack(blockTextureFolder / L"grass_block_side_overlay.png");
	snowyGrassBlockTexture = loadTextureFromResourcePack(blockTextureFolder / L"grass_block_snow.png");
	woolOverlay = loadTextureFromResourcePack(entityTextureFolder / L"sheep" / L"sheep_fur.png");
	endPortalFrameEyeTexture = loadTextureFromResourcePack(blockTextureFolder / L"end_portal_frame_eye.png");
	endSkyTexture = loadTextureFromResourcePack(environmentTextureFolder / L"end_sky.png");
	endCrystalTexture = loadTextureFromResourcePack(entityTextureFolder / L"end_crystal/end_crystal.png");
	endCrystalBeamTexture = loadTextureFromResourcePack(entityTextureFolder / L"end_crystal/end_crystal_beam.png");
	fireChargeTexture = loadTextureFromResourcePack(itemTextureFolder / L"fire_charge.png");
	dirtTexture = loadTextureFromResourcePack(blockTextureFolder / L"dirt.png");
	potionOverlayTexture = loadTextureFromResourcePack(itemTextureFolder / L"potion_overlay.png");
	rainTexture = loadTextureFromResourcePack(environmentTextureFolder / L"rain.png");
	snowTexture = loadTextureFromResourcePack(environmentTextureFolder / L"snow.png");
	sunTexture = loadTextureFromResourcePack(environmentTextureFolder / L"sun.png");
	moonPhasesTexture = loadTextureFromResourcePack(environmentTextureFolder / L"moon_phases.png");
	brewingStandBaseTexture = loadTextureFromResourcePack(blockTextureFolder / L"brewing_stand_base.png");
	brewingStandTopTexture = loadTextureFromResourcePack(blockTextureFolder / L"brewing_stand.png");
	enchantedItemTexture = loadTextureFromResourcePack(miscellaneousTextureFolder / L"enchanted_glint_item.png");
	unLitRedstoneTorchTexture = loadTextureFromResourcePack(blockTextureFolder / L"redstone_torch_off.png");
	redstoneLampOnTexture = loadTextureFromResourcePack(blockTextureFolder / L"redstone_lamp_on.png");

	const wstringContainer& furnaceNames{ std::wstring(L"furnace"), std::wstring(L"blast_furnace"), std::wstring(L"smoker") };

	for (int i = 0; i < furnaceTypeCount; i++)
	{
		furnaceOnTextures[i] = loadTextureFromResourcePack(blockTextureFolder / (furnaceNames[i] + std::wstring(L"_front_on.png")));
	}

	texture missingTextureGraphics(cveci2(2), false);
	missingTextureGraphics.setValueUnsafe(cvect2<size_t>(0), rgbColorValues[(int)colorID::pink]);
	missingTextureGraphics.setValueUnsafe(cvect2<size_t>(1), rgbColorValues[(int)colorID::pink]);
	missingTexture = new resolutionTexture(missingTextureGraphics);

	loadBlocks();

	// load destroy stage textures
	destroyStageTextures = std::vector<resolutionTexture*>();
	for (int i = 0; i < 9; i++)
	{
		resolutionTexture* destroyStageTexture = loadTextureFromResourcePack(blockTextureFolder / (std::wstring(L"destroy_stage_") + std::to_wstring(i) + std::wstring(L".png")));
		// const auto& destroyStageBrush = grayScaleToTransparency<texture>(destroyStageTexture, colorPalette::black);
		// destroyStageBrush.invert = true;
		// destroyStageTexture->fillRectangleUnsafe(destroyStageTexture->getClientRect(), destroyStageBrush);
		destroyStageTextures.push_back(destroyStageTexture);
	}

	loadItems();

	// add item data



	for (int i = 0; i < (int)colorID::count; i++)
	{
		itemList[(int)blockID::bed + i]->maxStackSize = 1;
	}
	itemList[(int)blockID::kelp]->tex = loadTextureFromResourcePack(itemTextureFolder / L"kelp.png");
	// add leave textures
	// color the texture
	for (int i = 0; i < normalTreeTypeCount; i++)
	{
		const blockID& idToColor = getTreeBlock((woodTypeID)i, treeItemTypeID::leaves);
		resolutionTexture* leavesInventoryGraphics = new resolutionTexture(texture(blockList[idToColor]->tex->scaledTextures[0]->size), cvec2(blockTextureSize));
		fillTransformedTexture(*blockList[idToColor]->tex, *leavesInventoryGraphics);

		constexpr color oakLeavesInventoryColor = hexToColor(0x48B518);
		multiplyRectangle(leavesInventoryGraphics->getClientRect(), oakLeavesInventoryColor, *leavesInventoryGraphics);
		itemList[(int)idToColor]->tex = leavesInventoryGraphics;
		loadedTextures.push_back(leavesInventoryGraphics);
	}

	/*
	for (int i = (int)blockID::wood_fence; i < (int)blockID::wood_fence + fenceTypeCount; i++)
	{
		resolutionTexture* renderedTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize)));

		cbool connect[fenceConnectionPossibilityCount]{ true, true };
		renderFence(blockTextureRect, connect, *blockList[i]->tex, *renderedTexture);
		itemList[i]->tex = renderedTexture;
	}
	for (int i = (int)blockID::wood_fence_gate; i < (int)blockID::wood_fence_gate + woodTypeCount; i++)
	{
		auto renderedTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize)));

		cbool connect[fenceConnectionPossibilityCount]{ true, false };
		renderFenceGate(blockTextureRect, connect, true, directionID::positiveX, *blockList[i]->tex, *renderedTexture);
		itemList[i]->tex = renderedTexture;
	}
	for (int i = (int)blockID::wood_slab; i < (int)blockID::wood_slab + slabTypeCount; i++)
	{
		auto renderedTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize)));

		cbool connect[fenceConnectionPossibilityCount]{ true, true };
		fillTransformedBrushRectangle(crectangle2(0, 0, blockTextureSize, blockTextureSize / 2), cveci2(), *blockList[i]->tex, *renderedTexture);
		itemList[i]->tex = renderedTexture;
	}
	*/

	setFoodValues();
	addFuelProperties();

	loadEntityData();

	resolutionTexture* spawnEggTexture = loadTextureFromResourcePack(itemTextureFolder / L"spawn_egg.png");
	resolutionTexture* spawnEggOverlayTexture = loadTextureFromResourcePack(itemTextureFolder / L"spawn_egg_overlay.png");
	for (int i = 0; i < mobTypeCount; i++)
	{
		entityID id = (entityID)i;
		mobData* data = (mobData*)entityDataList[(int)id];

		auto currentSpawnEggTexture = new resolutionTexture(texture(cvect2<fsize_t>(blockTextureSize)), cvec2(blockTextureSize));

		const solidColorBrush spawnEggColorBrush = solidColorBrush(data->spawnEggColor);
		const auto& spawnEggMultiplier = colorMultiplier<resolutionTexture, solidColorBrush>(*spawnEggTexture, spawnEggColorBrush);
		fillTransparentRectangle(crectangle2(currentSpawnEggTexture->getClientRect()), spawnEggMultiplier, *currentSpawnEggTexture);

		const solidColorBrush overLayColorBrush = solidColorBrush(data->spawnEggOverlayColor);
		const auto& overLayMultiplier = colorMultiplier<resolutionTexture, solidColorBrush>(*spawnEggOverlayTexture, overLayColorBrush);
		fillTransparentRectangle(crectangle2(currentSpawnEggTexture->getClientRect()), overLayMultiplier, *currentSpawnEggTexture);

		itemList[(int)itemID::spawn_egg + i] = new itemData((itemID)((int)itemID::spawn_egg + i), data->name + std::wstring(L"_spawn_egg"), currentSpawnEggTexture);
	}


	loadTags();
	loadLootTables();
	loadRecipes();
	readTrades();

	// add loot tables
	// for (const auto &fileIterator : stdFileSystem::directory_iterator(blockLootTablesFolder))
	//{
	//	std::wstring path = fileIterator.path().wstring();
	//	std::wstring fileNameWithExtension = fileIterator.path().filename().wstring();
	//	std::wstring fileNameWithoutExtension = fileNameWithExtension.substr(0, fileNameWithExtension.find(L'.'));
	//}

	dimensionDataList = idList<dimensionData*, dimensionID>(fastList<dimensionData*>((int)dimensionID::count));
	int currentDimensionID = 0;
	dimensionDataList[currentDimensionID] = new dimensionData(std::wstring(L"overworld"), loadTextureFromResourcePack(lightMapFolder / L"world0.png"), true);
	currentDimensionID++;
	dimensionDataList[currentDimensionID] = new dimensionData(std::wstring(L"nether"), loadTextureFromResourcePack(lightMapFolder / L"world-1.png"));
	currentDimensionID++;
	dimensionDataList[currentDimensionID] = new dimensionData(std::wstring(L"end"), loadTextureFromResourcePack(lightMapFolder / L"world1.png"));
	currentDimensionID++;

	idConverter::writeIDsToFile();

	loadStructures();

	reloadJigsawPools();
	stdPath textPath;
	if (getLastResourceLocation(textFolder / L"end.txt", textPath))
	{
		creditsText = stringToWString(readAllText(textPath));
		creditsText = replace(creditsText, std::wstring(L"PLAYERNAME"), std::wstring(L"Me"));
	}
}
std::vector<resolutionTexture*> loadGrowthStageTextures(const std::wstring& blockName, std::vector<size_t> growthStageIndexes)
{
	auto loadedTextures = std::vector<resolutionTexture*>();
	auto indexedTextures = std::vector<resolutionTexture*>(growthStageIndexes.size());
	for (size_t i = 0; i < growthStageIndexes.size(); i++)
	{
		csize_t& index = growthStageIndexes[i];
		if (index >= loadedTextures.size())
		{
			loadedTextures.push_back(loadTextureFromResourcePack(blockTextureFolder / (blockName + std::wstring(L"_stage") + std::to_wstring(index) + std::wstring(L".png"))));
		}
		indexedTextures[i] = loadedTextures[index];
	}

	return indexedTextures;
}
fp getItemWeakness(const itemID& identifier)
{
	if (isMaterialArmor(identifier))
	{
		return leatherArmorWeakness[getArmorType(identifier) - bootsArmorType] * armorTierWeaknessMultiplier[getArmorTier(identifier) - turtleArmorTier];
	}
	else if (isMaterialTool(identifier))
	{
		return toolWeakness[getMaterialToolTier(identifier) - woodHarvestTier];
	}
	else
	{
		switch (identifier)
		{
		case itemID::turtle_helmet:
			return leatherArmorWeakness[helmetArmorType - bootsArmorType] / armorTierWeaknessMultiplier[turtleArmorTier - turtleArmorTier];
		case itemID::fishing_rod:
		case itemID::flint_and_steel:
			return 1.0 / 0x40;
		case itemID::shears:
			return 1.0 / 0x100;
		case itemID::bow:
			return 1.0 / 0x180;
		case itemID::elytra:
			return 1.0 / 0x200;
		default:
			return -INFINITY;
		}
	}
}
std::shared_ptr<audioCollection> getEquipSound(const itemID& identifier)
{
	if (isMaterialArmor(identifier))
	{
		return armorEquipSound[(getArmorTier(identifier) - turtleArmorTier)];
	}
	else if (identifier == itemID::elytra)
	{
		return armorEquipSound[(leatherArmorTier - turtleArmorTier)];
	}
	else
	{
		return equipSound;
	}
}

void unloadResourcePacks()
{
	for (resolutionTexture* tex : loadedTextures)
	{
		delete tex;
	}
	loadedTextures = std::vector<resolutionTexture*>();
	chestLootTables.clear();
}

const resolutionTexture& replaceIfMissing(const resolutionTexture* const& textureToReplace)
{
	return textureToReplace ? *textureToReplace : *missingTexture;
}
resolutionTexture* loadChestTexture(const stdPath& path)
{
	resolutionTexture* chestImage = loadTextureFromResourcePack(path);
	cfp& scaleMultiplier = (fp)chestImage->scaledTextures[0]->size.x / chestImage->defaultSize.x;
	resolutionTexture* croppedTexture = new resolutionTexture(texture(cvect2<fsize_t>((fsize_t)((fp)blockTextureSize * scaleMultiplier))), cvec2(blockTextureSize));

	cfp& chestBottomPixelHeight = 10;
	cfp& padLockPixelWidth = 6;
	cfp& padLockPixelHeight = 5;
	fillTransformedBrushRectangle(crectangle2(0, 21, chestPixelWidth, chestBottomPixelHeight), cvec2((blockTextureSize - chestPixelWidth) / 2, 0), *chestImage, *croppedTexture);
	fillTransformedBrushRectangle(crectangle2(0, 45, chestPixelWidth, chestPixelHeight - chestBottomPixelHeight), cvec2((blockTextureSize - chestPixelWidth) / 2, chestBottomPixelHeight), *chestImage, *croppedTexture);
	fillTransparentRectangle(crectangle2(0, 59, padLockPixelWidth, padLockPixelHeight), crectangle2((blockTextureSize - (padLockPixelWidth / 2)) / 2, chestBottomPixelHeight - (padLockPixelHeight / 4), padLockPixelWidth / 2, padLockPixelHeight / 2), *chestImage, *croppedTexture);
	croppedTexture->recalculateScaledTextures();
	return croppedTexture;
}