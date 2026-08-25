#include <drogon/drogon.h>


#include "csv_loader.hpp"
#include "analytics.hpp"
#include "price_history.hpp"


int main()
{
    // get the data in listings.csv file
    std::vector<Listing> listings {
    loadListingsFromCsv("dashboard/data/listings.csv")
    };

// ============================================================================

    // Listings API
    drogon::app().registerHandler(
        "/api/listings",
        [listings](
            const drogon::HttpRequestPtr& request,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            std::vector<Listing> results {listings};

            std::string maxPriceParameter {
                request->getParameter("max_price")
            };

            std::string minBedroomsParameter{
                request->getParameter("min_bedrooms")
            };

            std::string minBathroomsParameter{
                request->getParameter("min_bathrooms")
            };

            if (!maxPriceParameter.empty())
            {
                double maxPrice {
                    std::stod(maxPriceParameter)
                };

                results = filterByMaxPrice(
                    results,
                    maxPrice
                );
            }

            if (!minBedroomsParameter.empty())
            {
                int minimumBedrooms {
                    std::stoi(minBedroomsParameter)
                };

                results = filterByBedrooms(
                    results,
                    minimumBedrooms
                );
            }

            if(!minBathroomsParameter.empty())
            {
                double minimumBathrooms {
                    std::stod(minBathroomsParameter)
                };
                
                results = filterByBathrooms(
                    results,
                    minimumBathrooms
                );
            }

            Json::Value jsonListings(Json::arrayValue);

            for (const Listing& listing : results)
            {
                Json::Value jsonListing;

                jsonListing["id"] = listing.id;
                jsonListing["source"] = listing.source;
                jsonListing["title"] = listing.title;
                jsonListing["price"] = listing.price;
                jsonListing["previousPrice"] = listing.previousPrice;
                jsonListing["neighborhood"] = listing.neighborhood;
                jsonListing["city"] = listing.city;
                jsonListing["bedrooms"] = listing.bedrooms;
                jsonListing["bathrooms"] = listing.bathrooms;
                jsonListing["url"] = listing.url;

                jsonListings.append(jsonListing);
            }

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(
                    jsonListings
                );

            callback(response);
        },
        {drogon::Get}
    );

// =========================================================================

drogon::app().registerHandler(
    "/api/neighborhood-averages",
    [listings](
        const drogon::HttpRequestPtr& request,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        auto averages =
            calculateAveragePriceByNeighborhood(listings);

        Json::Value jsonAverages(Json::arrayValue);

        for (const auto& [neighborhood, averagePrice] : averages)
        {
            Json::Value item;

            item["neighborhood"] = neighborhood;
            item["averagePrice"] = averagePrice;

            jsonAverages.append(item);
        }

        auto response =
            drogon::HttpResponse::newHttpJsonResponse(
                jsonAverages
            );

        callback(response);
    },
    {drogon::Get}
);
// =========================================================================

drogon::app().registerHandler(
    "/api/price-history",
    [](
        const drogon::HttpRequestPtr& request,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string url {
            request->getParameter("url")
        };

        if (url.empty())
        {
            Json::Value error;
            error["error"] = "Missing url parameter";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(
                    error
                );

            response->setStatusCode(
                drogon::k400BadRequest
            );

            callback(response);
            return;
        }

        std::vector<PriceHistoryEntry> history {
            loadPriceHistory(
                "dashboard/data/price_history.csv"
            )
        };

        std::vector<PriceHistoryEntry> listingHistory {
            getPriceHistoryForListing(
                history,
                url
            )
        };

        Json::Value jsonHistory(
            Json::arrayValue
        );

        for (const PriceHistoryEntry& entry : listingHistory)
        {
            Json::Value jsonEntry;

            jsonEntry["price"] =
                entry.price;

            jsonEntry["checkedAt"] =
                entry.checkedAt;

            jsonHistory.append(
                jsonEntry
            );
        }

        auto response =
            drogon::HttpResponse::newHttpJsonResponse(
                jsonHistory
            );

        callback(response);
    },
    {drogon::Get}
);

// =========================================================================

    drogon::app()
        .setDocumentRoot("dashboard/static")
        .addListener("127.0.0.1", 8080)
        .run();

    return 0;
}