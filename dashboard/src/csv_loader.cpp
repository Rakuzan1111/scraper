#include "csv_loader.hpp"
#include "csv_utils.hpp"

#include <fstream> //file stream: file -> program
#include <iostream>
#include <sstream>


// =======================================================================

// receives a filename and returns a vector of listing objects
std::vector<Listing> loadListingsFromCsv(
    const std::string& filename)
{
    std::ifstream file {filename};

    if (!file)
    {
        std::cerr
            << "Could not open file: "
            << filename
            << '\n';

        return {};
    }

    std::vector<Listing> listings;

    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::vector<std::string> fields {
            splitCsvLine(line)
        };

        if (fields.size() != 10)
        {
            std::cerr
                << "Skipping invalid listing row.\n";

            continue;
        }

        Listing listing;

        listing.id = fields[0];
        listing.source = "kijiji";

        listing.price =
            parseOptionalDouble(fields[1]);

        listing.title = fields[2];
        listing.location = fields[3];

        listing.bedrooms =
            parseOptionalInt(fields[4]);

        listing.bathrooms =
            parseOptionalDouble(fields[5]);

        listing.unitType = fields[6];

        listing.parking =
            parseOptionalInt(fields[7]);

        listing.sizeSqft =
            parseOptionalInt(fields[8]);

        listing.url = fields[9];

        listings.push_back(listing);
    }

    return listings;
}
// =====================================================================