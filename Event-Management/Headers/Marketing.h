#pragma once
#ifndef MARKETING_H
#define MARKETING_H
#include <vector>
#include "Event.h"       
using namespace std;

struct MarketingItem {
    string eventID;
    string name;
    double price;
    int quantity;
};

extern vector<MarketingItem> products;

void marketingUser();
void marketingAdmin();
vector<MarketingItem> selectProductsForEvent(const Event& ev);
void loadProductsFromFile();
void saveProductsToFile();



#endif
