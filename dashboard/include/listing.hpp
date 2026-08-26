#pragma once

#include <optional>
#include <string>

struct Listing
{
    std::string id;
    std::string source {"kijiji"};

    std::optional<double> price;
    std::optional<double> previousPrice;

    std::string title;
    std::string location;

    std::optional<int> bedrooms;
    std::optional<double> bathrooms;

    std::string unitType;

    std::optional<int> parking;
    std::optional<int> sizeSqft;

    std::string url;
};
