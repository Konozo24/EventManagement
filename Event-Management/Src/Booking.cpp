void Report::loadRegistration(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string regId, eventId, eventName, date, username, tickets, cost;

        getline(ss, regId, '|');
        getline(ss, eventId, '|');
        getline(ss, eventName, '|');
        getline(ss, date, '|');
        getline(ss, username, '|');
        getline(ss, tickets, '|');
        getline(ss, cost, '|');

        // Format as "EventName (EventID)"
        string fullEventName = eventName + " (" + eventId + ")";

        // Find if this event already exists in summaries
        bool found = false;
        for (auto& s : summaries) {
            if (s.eventName == fullEventName) {
                found = true;
                break;
            }
        }

        if (!found) {
            BookingSummary summary;
            summary.eventName = fullEventName;
            summary.date = date;

            // Resolve venue
            string venueID = eventVenueMap[eventId];
            if (!venueID.empty() && venueMap.find(venueID) != venueMap.end()) {
                summary.venue = venueMap[venueID];
            } else {
                summary.venue = "N/A";
            }

            summary.totalGuests = 0;
            summary.checkedInGuests = 0;

            summaries.push_back(summary);
        }
    }
    file.close();
}
