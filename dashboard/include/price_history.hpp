#pragma once

#include <string>
#include <vector>
#include <optional>

#include "listing.hpp"

struct PriceHistoryEntry
{
    std::string listingId;

    std::string url;
    std::string title;
    std::string location;

    std::optional<double> price {};

    std::string checkedAt;
};

std::vector<PriceHistoryEntry> loadPriceHistory(
    const std::string& filename);

std::vector<PriceHistoryEntry> getPriceHistoryForListing(
    const std::vector<PriceHistoryEntry>& history,
    const std::string& listingId);

std::optional<double> getPreviousPrice(
    const std::vector<PriceHistoryEntry>& listingHistory);

std::optional<double> parsePrice(
    const std::string& priceText
);

void attachPreviousPrices(
    std::vector<Listing>& listings,
    const std::vector<PriceHistoryEntry>& history);