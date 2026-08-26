// standard libraries
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <unordered_set>

// files
#include "analytics.hpp"
#include "csv_loader.hpp"
#include "discord_alert.hpp"
#include "alert_state.hpp"
#include "price_history.hpp"

int main()
{
    // testing variables
    int newAlertsSent{};
    int alreadyAlerted{};


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
    std::vector<Listing> listings {
        loadListingsFromCsv(
            "dashboard/data/listings.csv"
        )
    };

    
    std::vector<PriceHistoryEntry> history {
        loadPriceHistory(
            "dashboard/data/price_history.csv"
        )
    };

    attachPreviousPrices(
        listings,
        history
    );
    // filter only the listings whose price dropped
    std::vector<Listing> priceDropListings {
        findPriceDrops(listings)
    };

    // load the file with existing alerts
    const std::string alertStateFile{
        "dashboard/data/sent_alerts.txt"
    };
    std::unordered_set<std::string> sentAlertKeys {
        loadSentAlertKeys(alertStateFile)
    };



    //print all info
    std::cout << std::fixed << std::setprecision(2); //max two numbers after decimal

    std::cout 
        << "RENTAL ALERT CHECKER\n"
        << "====================\n"
        << "Price drops found: " << priceDropListings.size() << "\n\n";

    

    // list of listings
    for (const Listing& listing : priceDropListings)
    {
        // create a price drop alert key
        std::string alertKey{
            makePriceDropAlertKey(listing)
        };

        // checks if it already exists (if not, continue to next listing)
        if (sentAlertKeys.contains(alertKey))
        {
            std::cout << "Alert key already exists\n";
            ++alreadyAlerted;
            continue;
        }
        // create the discord message
        std::ostringstream message;
        message << std::fixed << std::setprecision(2);

        message
            << "RENTAL PRICE DROP\n\n"
            << listing.title << '\n'
            << "Previous price: $" << *listing.previousPrice << '\n'
            << "Current price: $" << *listing.price << '\n'
            << "Price drop: $" << calculatePriceDropAmount(listing) << '\n'
            << "Drop percent: " << calculatePriceDropPercent(listing) << "%\n\n"
            << listing.location << '\n'
            << listing.url;

        std::string messageText {
            message.str()
        };

        // send the discord message
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
            continue;
        }
        ++newAlertsSent;
        // save the alert key so message doesn't repeat itself if no price decrease between two scrapings
        bool saved {
            saveSentAlertKey(
                alertStateFile,
                alertKey
            )
        };

        if (!saved){
            std::cerr
                << "Alert sent, but failed to save alert state for: "
                << listing.title
                << '\n';
            continue;
        }
        sentAlertKeys.insert(alertKey);
    }
    std::cout
        << "\nPrice drops detected: " << priceDropListings.size() << '\n'
        << "New alerts sent: " << newAlertsSent << '\n'
        << "Already alerted: " << alreadyAlerted << '\n';

    curl_global_cleanup();

    return 0;
}