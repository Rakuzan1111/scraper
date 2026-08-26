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

std::unordered_map<std::string, double> calculateAveragePriceByLocation(
    const std::vector<Listing>& listings);

std::vector<Listing> filterByBedrooms(
    const std::vector<Listing>& listings,
    int minimumBedrooms);

std::vector<Listing> filterByBathrooms(
    const std::vector<Listing>& listings,
    double minimumBathrooms);

std::vector<Listing> findPriceDrops(
    const std::vector<Listing>& listings
    );

double calculatePriceDropAmount(
    const Listing& listing
    );
double calculatePriceDropPercent(
    const Listing& listing
    );