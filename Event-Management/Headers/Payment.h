#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <vector>
using namespace std;

struct Product {
    string name;
    double price;
    int quantity;
};

// Payment functions
void userCheckout(const string& name, const string& eventName, int ticketAmount, double ticketPrice,
                  const vector<Product>& selectedProducts);
void viewReceipts();

#endif
