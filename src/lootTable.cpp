#include "filesystem/textfile.h"
#include "lootTable.h"
#include "itemData.h"
#include "itemDrop.h"
#include "explosionDecayDrop.h"
#include "amountDrop.h"
#include "bonusAmountDistribution.h"
#include "oreDropsDistribution.h"
#include "binomialWithBonusCountDistribution.h"
#include "uniformBonusAmountDistribution.h"
#include "enchantmentData.h"
#include "alternativesDrop.h"
#include "uniformChoiceDrop.h"
#include "choiceDrop.h"
#include "rollDrop.h"
#include "conditionDrop.h"
#include "floatingSlot.h"
#include "additiveDrop.h"
#include "type/conversion.h"
#include "include/math/random/random.h"
#include "filesystem/jsonReader.h"

std::map<std::wstring, std::shared_ptr <lootTable>> chestLootTables = std::map<std::wstring, std::shared_ptr <lootTable>>();

std::shared_ptr <lootTable> readLootTable(const stdPath& path)
{
	const jsonContainer& container = readJson(stringToWString(readAllText(path)));
	if (path.wstring().contains(L"oak_leaves")) {
		stdPath copy = path;
	}
	std::shared_ptr<lootTable> table = std::shared_ptr <lootTable>(readDrop(container));
	return table;
}

//to understand loot tables:
//https://gist.github.com/misode/66456e57372ce62cd9b65d1052521069
lootTable* readDrop(const jsonContainer& container)
{
	lootTable* dropToReturn = nullptr;
	if (auto typeContainer = container.getChild(L"type"))
	{
		const std::wstring& typeName = typeContainer->children[0].value;
		//can be minecraft:block, minecraft:item, minecraft:gift etc. it doesn'T really matter.
		if (typeName == L"minecraft:item")
		{
			const itemID& itemIndex = itemList.getIDByName(container[L"name"].children[0].value);
			if ((int)itemIndex != -1)
			{
				dropToReturn = new itemDrop(itemIndex);
				csize_t& functionsIndex = container.getChildIndex(std::wstring(L"functions"));
				if (functionsIndex != std::wstring::npos)
				{
					const std::vector<jsonContainer>& functionContainers = container.children[functionsIndex].children;
					for (size_t i = 0; i < functionContainers.size(); i++)
					{
						const jsonContainer& functionContainer = functionContainers[i];
						const std::wstring functionName = functionContainer[L"function"].children[0].value;
						if (functionName == std::wstring(L"minecraft:set_count"))
						{
							const jsonContainer& countContainer = functionContainer[L"count"];
							dropToReturn = new amountDrop(dropToReturn, parseMinimumAndMaximum(countContainer));
						}
						else if (functionName == std::wstring(L"minecraft:explosion_decay"))
						{
							dropToReturn = new explosionDecayDrop(dropToReturn);
						}
						else if (functionName == std::wstring(L"minecraft:apply_bonus"))
						{
							const std::wstring formulaName = functionContainer[L"formula"].children[0].value;

							bonusAmountDistribution* bonus = nullptr;

							if (formulaName == std::wstring(L"minecraft:ore_drops"))
							{
								bonus = new oreDropsDistribution();
							}
							else
							{
								const jsonContainer& parameterContainer = functionContainer[L"parameters"];

								if (formulaName == std::wstring(L"minecraft:binomial_with_bonus_count"))
								{
									binomialWithBonusCountDistribution* distribution = new binomialWithBonusCountDistribution();
									convertTo<int>(parameterContainer[L"extra"].children[0].value, distribution->extra);
									convertTo<fp>(parameterContainer[L"probability"].children[0].value, distribution->probability);
									bonus = distribution;
								}
								else if (formulaName == std::wstring(L"minecraft:uniform_bonus_count"))
								{
									uniformBonusAmountDistribution* distribution = new uniformBonusAmountDistribution();
									convertTo<fp>(parameterContainer[L"bonusMultiplier"].children[0].value, distribution->bonusMultiplier);
									bonus = distribution;
								}
							}

							if (bonus)
							{
								std::wstring enchantmentName = functionContainer[L"enchantment"].children[0].value;
								bonus->enchantmentToUse = enchantmentDataList.getIDByName(enchantmentName);
							}
							dropToReturn = new amountDrop(dropToReturn, bonus);
						}
					}
				}
			}
			else
			{
				//item not added yet
				dropToReturn = new lootTable();
			}
		}
		else if (typeContainer->children[0].value == L"minecraft:alternatives")
		{
			alternativesDrop* drop = new alternativesDrop();
			const std::vector<jsonContainer> alternativeContainers = container[L"children"].children;
			for (const jsonContainer& alternative : alternativeContainers)
			{
				drop->alternatives.push_back(readDrop(alternative));
			}
			dropToReturn = drop;
		}
		else if (typeContainer->children[0].value == L"minecraft:empty")
		{
			dropToReturn = new lootTable();
		}
	}

	if (auto poolsContainer = container.getChild(L"pools"))
	{
		additiveDrop* drop = new additiveDrop();
		for (const jsonContainer& child : poolsContainer->children)
		{
			drop->drops.push_back(readDrop(child));
		}
		dropToReturn = drop;
	}

	csize_t& entriesIndex = container.getChildIndex(std::wstring(L"entries"));
	jsonContainer entriesContainer;
	if (entriesIndex != std::wstring::npos)
	{
		entriesContainer = container.children[entriesIndex];
	}
	else if (container.value == std::wstring(L"entries"))
	{
		entriesContainer = container;
	}
	else
	{
		goto noEntriesFound;
	}
	//add scope for goto label
	{
		choiceDrop* drop = new choiceDrop();
		for (const jsonContainer& child : entriesContainer.children)
		{
			fp chance;
			csize_t& weightIndex = child.getChildIndex(std::wstring(L"weight"));
			if ((weightIndex == std::wstring::npos) || !convertToDouble(child.children[weightIndex].children[0].value, chance))
			{
				chance = 1;
			}
			drop->options.push_back(readDrop(child));
			drop->chances.push_back(chance);
		}
		dropToReturn = drop;
	}
noEntriesFound:

	if (csize_t& rollsIndex = container.getChildIndex(std::wstring(L"rolls")); rollsIndex != std::wstring::npos)
	{
		const jsonContainer& rollsContainer = container.children[rollsIndex];

		rollDrop* drop = new rollDrop(dropToReturn);
		drop->distribution = parseMinimumAndMaximum(rollsContainer);
		dropToReturn = drop;
	}

	if (csize_t& conditionsIndex = container.getChildIndex(std::wstring(L"conditions")); conditionsIndex != std::wstring::npos)
	{
		const std::vector<jsonContainer> conditionContainers = container.children[conditionsIndex].children;
		for (size_t i = 0; i < conditionContainers.size(); i++)
		{
			dropToReturn = new conditionDrop(dropToReturn, readCondition(conditionContainers[i]));
		}
	}
	if (dropToReturn)
	{
		return dropToReturn;
	}
	else
	{
		return new lootTable();
		//handleError(std::wstring(L"can'T parse loot table");
	}
}
void lootTable::dropLoot(const std::vector<itemStack>& rolledStacks, tickableBlockContainer* containerIn, cvec2& position, cvec2& speed)
{
	//create entities
	for (itemStack s : rolledStacks)
	{
		floatingSlot* slot = (floatingSlot*)summonEntity(entityID::item, containerIn, position);
		slot->stack = s;
		slot->speed = speed;
	}
}

void lootTable::dropLoot(const std::vector<itemStack>& rolledStacks, tickableBlockContainer* containerIn, cvec2& position, cfp& maxSpeed)
{
	dropLoot(rolledStacks, containerIn, position, vec2::getrotatedvector(randFp(currentRandom, math::PI2)) * maxSpeed);
}