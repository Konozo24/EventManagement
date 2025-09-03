#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <vector>
#include "Registration.h"
#include "Event.h"
using namespace std;

// Payment functions
void processPayment(Registration& reg, Event* ev);
void viewReceipts();

#endif
