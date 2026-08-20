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