// standard libraries
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <curl/curl.h>
#include <sstream>

// files
#include "analytics.hpp"
#include "csv_loader.hpp"
#include "discord_alert.hpp"

int main()
{
    const char* webhookUrl {
        std::getenv(
            "DISCORD_WEBHOOK_URL"
        )
    };

    if (!webhookUrl)
    {
        std::cerr
            << "DISCORD_WEBHOOK_URL is not set.\n";

        return 1;
    }
    const CURLcode curlInit {
        curl_global_init(
            CURL_GLOBAL_DEFAULT
        )
    };

    if (curlInit != CURLE_OK)
    {
        std::cerr
            << "Failed to initialize CURL.\n";

        return 1;
    }

    // load the listings from our csv file
    std::vector<Listing>listings 
    {
    loadListingsFromCsv("dashboard/data/listings.csv")
    };


    // filter to only the listings whose price dropped
    std::vector<Listing>priceDropListings
    {
    findPriceDrops(listings)
    };

    //print all info
    std::cout << std::fixed << std::setprecision(2); //max two numbers after decimal

    std::cout 
        << "RENTAL ALERT CHECKER\n"
        << "====================\n"
        << "\nPrice drops found: " << priceDropListings.size() << '\n';

    

    // list of listings
    for (const Listing& listing : priceDropListings)
    {
        std::ostringstream message;
        message << std::fixed << std::setprecision(2);

        message
            << "RENTAL PRICE DROP\n\n"
            << listing.title << '\n'
            << "Previous price: $" << listing.previousPrice << '\n'
            << "Current price: $" << listing.price << '\n'
            << "Price drop: $" << calculatePriceDropAmount(listing) << '\n'
            << "Drop percent: " << calculatePriceDropPercent(listing) << "%\n\n"
            << listing.url;

        std::string messageText {
            message.str()
        };
        bool sent {
            sendDiscordMessage(
                webhookUrl,
                messageText
            )
        };
        if (!sent){
            std::cerr 
                << "Failed to send alert for: "
                << listing.title
                << '\n';
        }

    }

    curl_global_cleanup();

    return 0;
}