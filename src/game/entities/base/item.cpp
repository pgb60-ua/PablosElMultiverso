#include "../../game/entities/base/item.hpp"


Item::Item(string name, string description, Stats stats, ItemRarity itemRarity){
    this->name = name;
    this->description = description;
    this->stats = stats;
    this->itemRarity = itemRarity;
}