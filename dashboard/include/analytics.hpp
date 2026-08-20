#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "listing.hpp"

double calculateAveragePrice(
    const std::vector<Listing>& listings);

int countPriceDrops(
    const std::vector<Listing>& listings);

std::vector<Listing> filterByMaxPrice(
    const std::vector<Listing>& listings,
    double maxPrice);

std::unordered_map<std::string, double>
calculateAveragePriceByNeighborhood(
    const std::vector<Listing>& listings);