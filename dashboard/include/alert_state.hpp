#pragma once

#include<string>
#include<unordered_set>

#include "listing.hpp"

std::string makePriceDropAlertKey(
    const Listing& listing
);

std::unordered_set<std::string> loadSentAlertKeys(
    const std::string& filename
);

bool saveSentAlertKey(
    const std::string& filename,
    const std::string& key 
);