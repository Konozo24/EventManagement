#pragma once
#ifndef MONITORING_H
#define MONITORING_H

#include "Registration.h"
#include "Event.h"

void monitorEvent();
void displayCheckInStatistics(RegistrationManager& regManager);
void markVenueAsAvailable(VenueManager& venueManager, EventManager& eventManager);

#endif
