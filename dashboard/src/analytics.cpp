#include "analytics.hpp"

// =======================================================================

// calculates the average price of the apartments
double calculateAveragePrice(const std::vector<Listing>& listings)
{
    if (listings.empty())
    {
        return 0.0;
    }

    double total {};

    for (const Listing& listing : listings)
    {
        total += listing.price;
    }

    return total / listings.size();
}

// =======================================================================

// counts the number of apartments that dropped in price
int countPriceDrops(const std::vector<Listing>& listings)
{
    int count{};
    for (const Listing& listing : listings)
    {
        if (listing.price < listing.previousPrice)
        {
            ++count;
        }
    }
    return count;
}

// =======================================================================

// filters what is being printed based on max price set
std::vector<Listing> filterByMaxPrice(const std::vector<Listing>& listings, double maxPrice)
{
    std::vector<Listing> filteredListings;

    for (const Listing& listing : listings)
    {
        if (listing.price <= maxPrice)
        {
            filteredListings.push_back(listing);
        }
    }


    return filteredListings;
}

// =======================================================================

// calculates the average price of each neighborhood
std::unordered_map<std::string, double> calculateAveragePriceByNeighborhood(const std::vector<Listing>& listings)
{
    std::unordered_map<std::string, double> totals;
    std::unordered_map<std::string, int> counts;

    for (const Listing& listing : listings)
    {
        totals[listing.neighborhood] += listing.price;
        ++counts[listing.neighborhood];
    }

    std::unordered_map<std::string, double> averages;

    for (const auto& [neighborhood, total] : totals)
    {
        averages[neighborhood] =
            total / counts[neighborhood];
    }

    return averages;
}

// =======================================================================

// filters based on the minimum number of bedrooms
std::vector<Listing> filterByBedrooms(const std::vector<Listing>& listings, int minimumBedrooms){
    std::vector<Listing> filteredListings;
    for (const Listing& listing : listings){
        if (listing.bedrooms >= minimumBedrooms){
            filteredListings.push_back(listing);
        }
    }
    return filteredListings;
}

// =======================================================================

// filters based on minimum number of bathrooms
std::vector<Listing> filterByBathrooms(const std::vector<Listing>& listings, double minimumBathrooms){
    std::vector<Listing> filteredListings;
    for (const Listing&listing : listings){
        if (listing.bathrooms >= minimumBathrooms){
            filteredListings.push_back(listing);
        }
    }
    return filteredListings;
}

// =======================================================================

// filters based on if the price dropped
std::vector<Listing> findPriceDrops(const std::vector<Listing>& listings){
    std::vector<Listing> filteredListings;
    for (const Listing& listing : listings){
        if (listing.price < listing.previousPrice){
            filteredListings.push_back(listing);
        }
    }
    return filteredListings;
}

// =======================================================================

// calculates how much a listing's price dropped
double calculatePriceDropAmount(const Listing& listing){
    if (listing.price < listing.previousPrice){
        return listing.previousPrice - listing.price;
    }
    return 0.0;
}

// =======================================================================

// calculates how much percent the listing's price dropped
double calculatePriceDropPercent(const Listing& listing){
    if (listing.previousPrice <= 0){
        return 0.0;
    }

    if (listing.price < listing.previousPrice){
        return ((listing.previousPrice - listing.price)/listing.previousPrice)*100;
    }
    return 0.0;
}

// ======================================================================