#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <vector>
#include "Product.h"
#include "Registration.h"
using namespace std;

// Payment functions
void processPayment(Registration& reg);
void viewReceipts();

#endif
