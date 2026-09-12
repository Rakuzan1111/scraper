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
            splitCsvLine(line)
        };
            if (fields.size() != 5)
        {
            std::cerr << "Skipping invalid CSV row.\n";
            continue; // keyword to stop current loop and start next iteration 
        }
        PriceHistoryEntry entry;

        entry.listingId = fields[0];
        //entry.url = fields[1];
        entry.title = fields[1];
        entry.location = fields[2];
        entry.price = parseOptionalDouble(fields[3]);
        entry.checkedAt = fields[4];

        priceHistory.push_back(entry);
    }

    return priceHistory;
}

std::vector<PriceHistoryEntry> getPriceHistoryForListing(
    const std::vector<PriceHistoryEntry>& history,
    const std::string& listingId)
{
    std::vector<PriceHistoryEntry> specificListing;
    for (const PriceHistoryEntry& entry: history){
        if(entry.listingId == listingId){
            specificListing.push_back(entry);
        }
    }
    return specificListing;
}

std::optional<double> getPreviousPrice(
    const std::vector<PriceHistoryEntry>& listingHistory)
{
    if (listingHistory.size() < 2)
    {
        return std::nullopt;
    }

    for (std::size_t i = listingHistory.size() - 1;
         i > 0;
         --i)
    {
        const PriceHistoryEntry& previousEntry {
            listingHistory[i - 1]
        };

        if (previousEntry.price.has_value())
        {
            return previousEntry.price;
        }
    }

    return std::nullopt;
}

std::optional<double> parsePrice(
    const std::string& priceText
)
{

    if(priceText == "Please Contact" || priceText == "N/A" || priceText == ""){
        return std::nullopt;
    }
    std::string cleanedPrice;

    for (char character : priceText)
    {
        if ((character >= '0' && character <= '9') ||
            character == '.')
        {
            cleanedPrice += character;
        }
    }
    if (cleanedPrice.empty()){
        return std::nullopt;
    }
    try
    {
        return std::stod(cleanedPrice);
    }
    catch(...)
    {
        return std::nullopt;
    }
}

void attachPreviousPrices(
    std::vector<Listing>& listings,
    const std::vector<PriceHistoryEntry>& history)
{
    for (Listing& listing : listings)
    {
        const std::vector<PriceHistoryEntry> listingHistory {
            getPriceHistoryForListing(
                history,
                listing.id
            )
        };

        listing.previousPrice =
            getPreviousPrice(listingHistory);
    }
}