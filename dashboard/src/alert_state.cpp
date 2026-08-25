#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "alert_state.hpp"

// =======================================================

//creates a unique drop alert key for every listing
std::string makePriceDropAlertKey(const Listing& listing)
{
    std::ostringstream alertKey;
    alertKey << std::fixed << std::setprecision(2);

    alertKey 
        << listing.source 
        << '|'
        << listing.id
        << '|'
        << listing.previousPrice
        << '|'
        << listing.price;

    return alertKey.str();
}

// ========================================================

//loads previously sent alerts
std::unordered_set<std::string> loadSentAlertKeys(const std::string& filename)
{
    std::ifstream file {filename};
    if (!file){
        return {};
    }
    std::string line;
    std::unordered_set<std::string> alerts;
    
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            alerts.insert(line);
        }
    }
    return alerts;
}

// ==========================================================

//adds alert keys to the file
bool saveSentAlertKey(
    const std::string& filename,
    const std::string& key)
{
    std::ofstream alertFile {
        filename,
        std::ios::app
    };

    if (!alertFile)
    {
        std::cerr
            << "Unable to open file: "
            << filename
            << '\n';

        return false;
    }

    alertFile << key << '\n';

    if (!alertFile)
    {
        std::cerr
            << "Unable to write alert key.\n";

        return false;
    }

    return true;
}