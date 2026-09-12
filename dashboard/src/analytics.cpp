#include "analytics.hpp"

// =======================================================================

// calculates the average price of the apartments
double calculateAveragePrice(
    const std::vector<Listing>& listings)
{
    double total {};
    std::size_t count {};

    for (const Listing& listing : listings)
    {
        if (listing.price.has_value())
        {
            total += *listing.price;
            ++count;
        }
    }

    if (count == 0)
    {
        return 0.0;
    }

    return total / count;
}

// =======================================================================

// counts the number of apartments that dropped in price
int countPriceDrops(
    const std::vector<Listing>& listings)
{
    int count {};

    for (const Listing& listing : listings)
    {
        if (listing.price.has_value() &&
            listing.previousPrice.has_value() &&
            *listing.price < *listing.previousPrice)
        {
            ++count;
        }
    }

    return count;
}

// =======================================================================

// filters what is being printed based on max price set
std::vector<Listing> filterByMaxPrice(
    const std::vector<Listing>& listings,
    double maxPrice)
{
    std::vector<Listing> filteredListings;

    for (const Listing& listing : listings)
    {
        if (listing.price.has_value() &&
            *listing.price <= maxPrice)
        {
            filteredListings.push_back(listing);
        }
    }

    return filteredListings;
}

// =======================================================================

// calculates the average price of each neighborhood
std::unordered_map<std::string, double>
calculateAveragePriceByLocation(
    const std::vector<Listing>& listings)
{
    std::unordered_map<std::string, double> totals;
    std::unordered_map<std::string, int> counts;

    for (const Listing& listing : listings)
    {
        if (!listing.price.has_value() ||
            listing.location.empty())
        {
            continue;
        }

        totals[listing.location] +=
            *listing.price;

        ++counts[listing.location];
    }

    std::unordered_map<std::string, double> averages;

    for (const auto& [location, total] : totals)
    {
        averages[location] =
            total / counts[location];
    }

    return averages;
}

// =======================================================================

// filters based on the minimum number of bedrooms
std::vector<Listing> filterByBedrooms(
    const std::vector<Listing>& listings,
    int minimumBedrooms)
{
    std::vector<Listing> filteredListings;

    for (const Listing& listing : listings)
    {
        if (listing.bedrooms.has_value() &&
            *listing.bedrooms >= minimumBedrooms)
        {
            filteredListings.push_back(listing);
        }
    }

    return filteredListings;
}

// =======================================================================

// filters based on minimum number of bathrooms
std::vector<Listing> filterByBathrooms(
    const std::vector<Listing>& listings,
    double minimumBathrooms)
{
    std::vector<Listing> filteredListings;

    for (const Listing& listing : listings)
    {
        if (listing.bathrooms.has_value() &&
            *listing.bathrooms >= minimumBathrooms)
        {
            filteredListings.push_back(listing);
        }
    }

    return filteredListings;
}

// =======================================================================

// filters based on if the price dropped
std::vector<Listing> findPriceDrops(
    const std::vector<Listing>& listings)
{
    std::vector<Listing> priceDrops;

    for (const Listing& listing : listings)
    {
        if (listing.price.has_value() &&
            listing.previousPrice.has_value() &&
            *listing.price < *listing.previousPrice)
        {
            priceDrops.push_back(listing);
        }
    }

    return priceDrops;
}

// =======================================================================

// calculates how much a listing's price dropped
double calculatePriceDropAmount(
    const Listing& listing)
{
    if (!listing.price.has_value() ||
        !listing.previousPrice.has_value())
    {
        return 0.0;
    }

    if (*listing.price >= *listing.previousPrice)
    {
        return 0.0;
    }

    return
        *listing.previousPrice -
        *listing.price;
}

// =======================================================================

// calculates how much percent the listing's price dropped
double calculatePriceDropPercent(
    const Listing& listing)
{
    if (!listing.price.has_value() ||
        !listing.previousPrice.has_value())
    {
        return 0.0;
    }

    if (*listing.previousPrice <= 0.0 ||
        *listing.price >= *listing.previousPrice)
    {
        return 0.0;
    }

    return
        ((*listing.previousPrice - *listing.price)
        / *listing.previousPrice)
        * 100.0;
}
// ======================================================================