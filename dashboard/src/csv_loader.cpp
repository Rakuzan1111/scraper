#include "csv_loader.hpp"

#include <fstream> //file stream: file -> program
#include <iostream>
#include <sstream>

// takes a string and creates a vector (list of only numbers = python equivalent) of the elements separated by commas
// ex. "I,am,John,Doe" -> {I, am, John, Doe}
std::vector<std::string> splitLine(const std::string& line)
{
    std::vector<std::string> fields;
    std::stringstream stream {line};
    std::string field;

    while (std::getline(stream, field, ','))
    {
        fields.push_back(field);
    }

    return fields;
}

// =======================================================================

// receives a filename and returns a vector of listing objects
std::vector<Listing> loadListingsFromCsv(const std::string& filename)
{
    std::ifstream file {filename};

    if (!file)
    {
        std::cerr << "Could not open file: " //cerr for error messages
                  << filename
                  << '\n';

        return {};
    }

    std::vector<Listing> listings;

    std::string line;

    std::getline(file, line); //initial read for the first line as it is not necessary (column names)

    while (std::getline(file, line))
    {
        std::vector<std::string> fields {
            splitLine(line)
        };

        if (fields.size() != 10)
        {
            std::cerr << "Skipping invalid CSV row.\n";
            continue; // keyword to stop current loop and start next iteration 
        }

        Listing listing;

        listing.id = fields[0];
        listing.source = fields[1];
        listing.title = fields[2];
        listing.price = std::stod(fields[3]); //stod = string to double
        listing.previousPrice = std::stod(fields[4]);
        listing.neighborhood = fields[5];
        listing.city = fields[6];
        listing.bedrooms = std::stoi(fields[7]); //stoi = string to integer
        listing.bathrooms = std::stod(fields[8]);
        listing.url = fields[9];

        listings.push_back(listing);
    }

    return listings;
}

// =====================================================================