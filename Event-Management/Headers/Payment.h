#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <vector>
#include "Guest.h"       // To use Guest info
#include "Constants.h"   // For RECEIPT_FILE constant

struct Product {
    std::string name;
    double price;
    int quantity;
};

// Core payment functions
void userCheckout(const std::string& name, const std::string& eventName, 
                  int ticketAmount, double ticketPrice, 
                  const std::vector<Product>& selectedProducts);

void viewReceipts();

// Helpers
std::string buildProductSummary(const std::vector<Product>& selectedProducts, double& totalProductPrice);
int getNextReceiptNumber();
void saveReceipt(const std::string& name, const std::string& eventName,
                 const std::string& productSummary, int ticketAmount, 
                 double ticketPrice, double productPrice, const std::string& method);

#endif
