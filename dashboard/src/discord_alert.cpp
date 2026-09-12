#include "discord_alert.hpp"

#include <curl/curl.h>
#include <json/json.h>

#include <iostream>
#include <string>


bool sendDiscordMessage(
    const std::string& webhookUrl,
    const std::string& message)
{
    Json::Value payload;

    payload["content"] = message;

    Json::StreamWriterBuilder writer;

    writer["indentation"] = "";

    const std::string jsonBody {
        Json::writeString(
            writer,
            payload
        )
    };


    CURL* curl {
        curl_easy_init()
    };

    if (!curl)
    {
        std::cerr
            << "Failed to create CURL handle.\n";

        return false;
    }


    curl_slist* headers {nullptr};

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );


    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        webhookUrl.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        jsonBody.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDSIZE,
        static_cast<long>(
            jsonBody.size()
        )
    );

    curl_easy_setopt(
        curl,
        CURLOPT_TIMEOUT,
        10L
    );


    const CURLcode result {
        curl_easy_perform(curl)
    };


    long statusCode {};

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &statusCode
    );


    curl_slist_free_all(headers);

    curl_easy_cleanup(curl);


    if (result != CURLE_OK)
    {
        std::cerr
            << "Discord request failed: "
            << curl_easy_strerror(result)
            << '\n';

        return false;
    }


    if (statusCode < 200 ||
        statusCode >= 300)
    {
        std::cerr
            << "Discord returned HTTP "
            << statusCode
            << '\n';

        return false;
    }


    return true;
}