#pragma once

#include <optional>
#include <string>
#include <vector>

std::vector<std::string> splitCsvLine(
    const std::string& line);

std::optional<double> parseOptionalDouble(
    const std::string& text);

std::optional<int> parseOptionalInt(
    const std::string& text);