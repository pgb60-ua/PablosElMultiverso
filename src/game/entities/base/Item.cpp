#include "Item.hpp"

Item::Item(std::string name, std::string description, Stats stats, ItemRarity itemRarity, int price)
    : name(name), description(description), stats(stats), itemRarity(itemRarity), price(price)
{
}
