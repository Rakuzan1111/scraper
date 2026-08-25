//standard libraries
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <unordered_map> //very similar to a dictionary for python (without order)

//files
#include "listing.hpp"
#include "analytics.hpp"
#include "csv_loader.hpp"
#include "price_history.hpp"

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
    std::vector<Listing> priceDropListings {
    findPriceDrops(listings)
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

    std::cout << "=============================\n";

    //print what is being filtered (apartments under 2000$)
    std::cout << "\nListings at or under $" << maxPrice << ":\n";
    for (const Listing& listing : affordableListings)
    {
        std::cout << listing.title << " - $" << listing.price << '\n';
    }

    std::cout << "=============================\n";

    //print average price per neighborhood
    std::cout << "\nAverage rent by neighborhood:\n";

    for (const auto& [neighborhood, average] : neighborhoodAverages)
    {
        std::cout << neighborhood << ": $" << average << '\n';
    }

    std::cout << "=============================\n";

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

    std::cout << "=============================\n";

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

    std::cout << "=============================\n";
    
    //print listings based on price drops
    std::cout << "\nListings that dropped in price:\n";

    std::cout << std::fixed << std::setprecision(2);
    for (const Listing&listing : priceDropListings)
    {
        std::cout
            << "--------------------\n"
            << listing.title << '\n'
            << "Previous: $" << listing.previousPrice << '\n'
            << "Current: $" << listing.price << '\n'
            << "Drop: $" << calculatePriceDropAmount(listing) <<'\n'
            << "Drop %: " << calculatePriceDropPercent(listing) <<"%\n"
            << '\n';
    }

    //print price history
    std::vector<PriceHistoryEntry> history {loadPriceHistory("dashboard/data/price_history.csv")};
    std::vector<PriceHistoryEntry> filteredHistory {getPriceHistoryForListing(history, "https://example.com/A001")};
    std::cout 
        << "PRICE HISTORY TEST\n"
        << "==================\n"
        << "Total history entries: " << filteredHistory.size() << '\n'
        << "History for A001:\n";
    
    for (const PriceHistoryEntry& entry: filteredHistory)
    {
        std::cout
            << entry.checkedAt << " - " << entry.price << '\n';
    }
    std::cout 
        << "\nPrevious price: $" << getPreviousPrice(filteredHistory)
        << "\nCurrent price: $" << filteredHistory.back().price;

    return 0;
}