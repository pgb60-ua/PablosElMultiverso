#pragma once

#include "Item.hpp"
#include <memory>
#include <string>
#include <vector>
class ItemsFactory
{
public:
    static ItemsFactory &GetInstance()
    {
        static ItemsFactory instance;
        return instance;
    }
    ItemsFactory();
    ~ItemsFactory();
    void LoadAllItems();
    const Item *GetItem(const std::string &itemid) const;
    std::vector<const Item *> GetRandomItems(int count);

private:
    std::vector<std::unique_ptr<Item>> allItems;
};
