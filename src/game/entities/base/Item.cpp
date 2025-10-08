#include <Item.hpp>

Item::Item(std::string name, std::string description, Stats stats, ItemRarity itemRarity)
    : name(name), description(description), stats(stats), itemRarity(itemRarity) {}