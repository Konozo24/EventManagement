#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <vector>
#include "Product.h"
using namespace std;

// Payment functions
void userCheckout(const string& name, const string& eventName, int ticketAmount, double ticketPrice,
                  const vector<Product>& selectedProducts);
void viewReceipts();

#endif
