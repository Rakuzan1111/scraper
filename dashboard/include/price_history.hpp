#pragma once

#include <string>
#include <vector>

struct PriceHistoryEntry
{
    std::string url;
    std::string title;
    std::string location;

    double price {};

    std::string checkedAt;
};

std::vector<PriceHistoryEntry> loadPriceHistory(
    const std::string& filename);

std::vector<PriceHistoryEntry> getPriceHistoryForListing(
    const std::vector<PriceHistoryEntry>& history,
    const std::string& url);

double getPreviousPrice(
    const std::vector<PriceHistoryEntry>& listingHistory);