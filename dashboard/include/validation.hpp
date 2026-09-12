#pragma once

#include "listing.hpp"

#include <string>
#include <vector>


struct ValidationIssue
{
    std::string field;
    std::string message;
};


std::vector<ValidationIssue> validateListing(
    const Listing& listing
);