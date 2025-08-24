#pragma once
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

struct Product {
    string name;
    double price;
    int quantity;

    Product() : name(""), price(0.0), quantity(0) {}
    Product(const string& n, double p, int q = 0)
        : name(n), price(p), quantity(q) {}
};

#endif
