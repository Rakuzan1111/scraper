#pragma once

#include <string>
#include <vector>

#include "listing.hpp"

std::vector<Listing> loadListingsFromCsv(
    const std::string& filename);