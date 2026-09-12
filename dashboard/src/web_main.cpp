#include <drogon/drogon.h>


#include "csv_loader.hpp"
#include "analytics.hpp"
#include "price_history.hpp"


int main()
{
    // get the data in listings.csv file
    std::vector<Listing> listings {
        loadListingsFromCsv(
            "listings.csv"
        )
    };

    std::vector<PriceHistoryEntry> history {
        loadPriceHistory(
            "price_history.csv"
        )
    };

    attachPreviousPrices(
        listings,
        history
    );

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
                jsonListing["location"] = listing.location;
                jsonListing["unitType"] = listing.unitType;
                if (listing.price.has_value())
                {
                    jsonListing["price"] = *listing.price;
                }
                else
                {
                    jsonListing["price"] = Json::nullValue;
                }
                if (listing.previousPrice.has_value())
                {
                    jsonListing["previousPrice"] =
                        *listing.previousPrice;
                }
                else
                {
                    jsonListing["previousPrice"] =
                        Json::nullValue;
                }
                if (listing.bedrooms.has_value())
                {
                    jsonListing["bedrooms"] =
                        *listing.bedrooms;
                }
                else
                {
                    jsonListing["bedrooms"] =
                        Json::nullValue;
                }           
                if (listing.bathrooms.has_value())
                {
                    jsonListing["bathrooms"] =
                        *listing.bathrooms;
                }
                else
                {
                    jsonListing["bathrooms"] =
                        Json::nullValue;
                }
                if (listing.parking.has_value())
                {
                    jsonListing["parking"] =
                        *listing.parking;
                }
                else
                {
                    jsonListing["parking"] =
                        Json::nullValue;
                }
                if (listing.sizeSqft.has_value())
                {
                    jsonListing["sizeSqft"] =
                        *listing.sizeSqft;
                }
                else
                {
                    jsonListing["sizeSqft"] =
                        Json::nullValue;
                }
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
    "/api/location-averages",
    [listings](
        const drogon::HttpRequestPtr& request,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        auto averages =
            calculateAveragePriceByLocation(listings);

        Json::Value jsonAverages(Json::arrayValue);

        for (const auto& [location, averagePrice] : averages)
        {
            Json::Value item;

            item["location"] = location;
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
        std::string listingId {
            request->getParameter("listing_id")
        };

        if (listingId.empty())
        {
            Json::Value error;
            error["error"] = "Missing listing_id parameter";

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
                "price_history.csv"
            )
        };

        std::vector<PriceHistoryEntry> listingHistory {
            getPriceHistoryForListing(
                history,
                listingId
            )
        };

        Json::Value jsonHistory(
            Json::arrayValue
        );

        for (const PriceHistoryEntry& entry : listingHistory)
        {
            Json::Value jsonEntry;

            if (entry.price.has_value())
            {
                jsonEntry["price"] =
                    *entry.price;
            }
            else
            {
                jsonEntry["price"] =
                    Json::nullValue;
            }

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