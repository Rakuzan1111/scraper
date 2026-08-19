#include <iostream>
#include <vector>

#include "listing.hpp"

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
int main()
{

    // list of apartment details (dummy information)


    Listing apartment;

    apartment.id = "A001";
    apartment.source = "test-site";
    apartment.title = "2 Bedroom Downtown Apartment";
    apartment.price = 1850.0;
    apartment.previousPrice = 1950.0;
    apartment.neighborhood = "Downtown";
    apartment.city = "Montreal";
    apartment.bedrooms = 2;
    apartment.bathrooms = 1.0;
    apartment.url = "https://example.com/A001";

    // ===================================================================

    Listing apartment2;

    apartment2.id = "A002";
    apartment2.source = "test-site";
    apartment2.title = "Plateau Studio";
    apartment2.price = 1450.0;
    apartment2.previousPrice = 1350;
    apartment2.neighborhood = "Plateau";
    apartment2.city = "Montreal";
    apartment2.bedrooms = 0;
    apartment2.bathrooms = 1.0;
    apartment2.url = "https://example.com/A002";

    // ===================================================================

    Listing apartment3;

    apartment3.id = "A003";
    apartment3.source = "test-site";
    apartment3.title = "Verdun 3 Bedroom Apartment";
    apartment3.price = 2200;
    apartment3.previousPrice = 2400;
    apartment3.neighborhood = "Verdun";
    apartment3.city = "Montreal";
    apartment3.bedrooms = 3;
    apartment3.bathrooms = 2;
    apartment3.url = "https://example.com/A003";

    // ===================================================================

    // print details of the apartments
    std::vector<Listing> listings;
    listings.push_back(apartment);
    listings.push_back(apartment2);
    listings.push_back(apartment3);

    // asks the user their maximum price to filter out apartments above the rent price
    double maxPrice {};
    std::cout << "\nEnter maximum rent: ";
    std::cin >> maxPrice;
    std::vector<Listing> affordableListings {filterByMaxPrice(listings, maxPrice)};

    // variables to display
    double averagePrice { calculateAveragePrice(listings)};
    int priceDrops {countPriceDrops(listings)};


    for (const Listing& listing : listings)
    {
        // print the information of each listings element
        std::cout << "--------------\n";
        std::cout << "Title: " << listing.title << '\n';
        std::cout << "Price: $" << listing.price << '\n';
        std::cout << "Neighborhood: " << listing.neighborhood << '\n';
        std::cout << "Bedrooms: " << listing.bedrooms << '\n';
        std::cout << "Bathrooms: " << listing.bathrooms << '\n';
    }
    std::cout << "\nTotal listings: " << listings.size() << '\n';

    // print average price
    std::cout << "\nAverage Rent: $" << averagePrice << '\n';

    //print number of apartments that dropped in price
    std::cout << "\nPrice drops: " << priceDrops << '\n';

    //print what is being filtered (apartments under 2000$)
    std::cout << "\nListings at or under $" << maxPrice << ":\n";
    for (const Listing& listing : affordableListings)
    {
        std::cout << listing.title << " - $" << listing.price << '\n';
    }

    return 0;
}