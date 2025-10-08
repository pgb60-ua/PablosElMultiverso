#include <item.hpp>


Item::Item(const std::string& name, const std::string& description, const Stats& stats, ItemRarity itemRarity){
    this->name = name;
    this->description = description;
    this->stats = stats;
    this->itemRarity = itemRarity;
}