#pragma once
#ifndef MARKETING_H
#define MARKETING_H
#include <vector>
#include "Event.h"       
#include "Product.h"
using namespace std;

struct MarketingItem {
    string eventID;
    string name;
    double price;
    int quantity;
};

class MarketingManager {
private:
    vector<MarketingItem> products;

public:

    void marketingUser();
    void marketingAdmin();
    vector<MarketingItem> selectProductsForEvent(const Event& ev);
    void loadProductsFromFile();
    void saveProductsToFile();

    vector<MarketingItem>& getProducts() { return products; }
};


#endif
