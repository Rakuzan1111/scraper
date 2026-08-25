#include <fstream> //file stream: file -> program
#include <iostream>
#include <sstream>
#include "csv_utils.hpp"

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
