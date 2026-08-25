#include <fstream>
#include <iostream>

#include "price_history.hpp"
#include "csv_loader.hpp"


std::vector<PriceHistoryEntry> loadPriceHistory(
    const std::string& filename)
{
    std::ifstream file {filename};
    if (!file)
    {
        std::cerr 
            << "Could not open file: " 
            << filename 
            << '\n';
        return {};
    }

    std::vector<PriceHistoryEntry> priceHistory;
    std::string line;
    std::getline(file, line);
    while(std::getline(file, line))
    {
        std::vector<std::string> fields {
            splitLine(line)
        };
            if (fields.size() != 5)
        {
            std::cerr << "Skipping invalid CSV row.\n";
            continue; // keyword to stop current loop and start next iteration 
        }
        PriceHistoryEntry entry;

        entry.url = fields[0];
        entry.title = fields[1];
        entry.location = fields[2];
        entry.price = std::stod(fields[3]);
        entry.checkedAt = fields[4];

        priceHistory.push_back(entry);
    }

    return priceHistory;
}

std::vector<PriceHistoryEntry> getPriceHistoryForListing(
    const std::vector<PriceHistoryEntry>& history,
    const std::string& url)
{
    std::vector<PriceHistoryEntry> specificListing;
    for (const PriceHistoryEntry& entry: history){
        if(entry.url == url){
            specificListing.push_back(entry);
        }
    }
    return specificListing;
}

double getPreviousPrice(
    const std::vector<PriceHistoryEntry>& listingHistory)
{
    if (listingHistory.size() < 2){
        return 0.0;
    }

    const PriceHistoryEntry& previousEntry {
        listingHistory[listingHistory.size() - 2]
    };
    return previousEntry.price;
}