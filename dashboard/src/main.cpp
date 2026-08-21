//standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map> //very similar to a dictionary for python (without order)

//files
#include "listing.hpp"
#include "analytics.hpp"
#include "csv_loader.hpp"

// =======================================================================

int main()
{

    std::vector<Listing> listings {
    loadListingsFromCsv("dashboard/data/listings.csv")
    };
    // asks the user their maximum price to filter out apartments above the rent price
    double maxPrice {};
    std::cout << "\nEnter maximum rent: ";
    std::cin >> maxPrice;
    std::vector<Listing> affordableListings {filterByMaxPrice(listings, maxPrice)};

    // variables to display
    double averagePrice { calculateAveragePrice(listings)};
    int priceDrops {countPriceDrops(listings)};
    std::unordered_map<std::string, double> neighborhoodAverages{calculateAveragePriceByNeighborhood(listings)};
    std::vector<Listing> bedroomListings {
    filterByBedrooms(listings, 2)
    };
    std::vector<Listing> bathroomListings {
    filterByBathrooms(listings, 1)
    };

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

    //print average price per neighborhood
    std::cout << "\nAverage rent by neighborhood:\n";

    for (const auto& [neighborhood, average] : neighborhoodAverages)
    {
        std::cout << neighborhood << ": $" << average << '\n';
    }

    //print listings based on minimum bedrooms
    std::cout << "\nListings with 2+ bedrooms:\n";

    for (const Listing& listing : bedroomListings)
    {
        std::cout
            << listing.title
            << " - "
            << listing.bedrooms
            << " bedrooms - $"
            << listing.price
            << '\n';
    }

    //print listings based on minimum bathrooms
    std::cout << "\nListings with 1+ bathrooms:\n";
    
    for (const Listing&listing : bedroomListings)
    {
        std::cout
            << listing.title
            << " - "
            << listing.bathrooms
            << " bathrooms - $"
            << listing.price
            << '\n';
    }
    
    return 0;
}