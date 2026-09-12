#pragma once

#include <string>

bool sendDiscordMessage(
    const std::string& webhookUrl,
    const std::string& message);