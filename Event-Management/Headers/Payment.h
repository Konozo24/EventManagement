#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <vector>
#include "Product.h"
using namespace std;

// Payment functions
void processPayment(const string& guestID);
void viewReceipts();

#endif
