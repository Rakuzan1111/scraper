#pragma once

#include <string>
#include <vector>

#include "listing.hpp"
#include "csv_utils.hpp"

std::vector<std::string> splitLine(const std::string& line);

std::vector<Listing> loadListingsFromCsv(
    const std::string& filename);