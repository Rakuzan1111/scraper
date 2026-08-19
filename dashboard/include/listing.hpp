#pragma once

#include <string>

struct Listing
{
    std::string id;
    std::string source;
    std::string title;
    double price {};
    double previousPrice {};
    std::string neighborhood;
    std::string city;
    int bedrooms {};
    double bathrooms {};
    std::string url;
};
