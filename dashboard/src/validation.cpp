#include "validation.hpp"


std::vector<ValidationIssue> validateListing(
    const Listing& listing
)
{
    std::vector<ValidationIssue> issues;

    // --------------------------------
    // Required text fields
    // --------------------------------

    if (listing.id.empty())
    {
        issues.push_back({
            "id",
            "Listing ID is missing"
        });
    }

    if (listing.title.empty())
    {
        issues.push_back({
            "title",
            "Listing title is missing"
        });
    }

    if (listing.location.empty())
    {
        issues.push_back({
            "location",
            "Listing location is missing"
        });
    }

    if (listing.url.empty())
    {
        issues.push_back({
            "url",
            "Listing URL is missing"
        });
    }
    else
    {
        bool startsWithHttp =
            listing.url.rfind("http://", 0) == 0;

        bool startsWithHttps =
            listing.url.rfind("https://", 0) == 0;

        if (!startsWithHttp && !startsWithHttps)
        {
            issues.push_back({
                "url",
                "Listing URL does not begin with http:// or https://"
            });
        }
    }


    // --------------------------------
    // Price validation
    // --------------------------------

    if (listing.price.has_value())
    {
        if (*listing.price < 300)
        {
            issues.push_back({
                "price",
                "Price is suspiciously low"
            });
        }

        if (*listing.price > 10000)
        {
            issues.push_back({
                "price",
                "Price is suspiciously high"
            });
        }
    }


    // --------------------------------
    // Bedroom validation
    // --------------------------------

    if (listing.bedrooms.has_value())
    {
        if (*listing.bedrooms < 0)
        {
            issues.push_back({
                "bedrooms",
                "Bedroom count cannot be negative"
            });
        }

        if (*listing.bedrooms > 10)
        {
            issues.push_back({
                "bedrooms",
                "Bedroom count is unusually high"
            });
        }
    }


    // --------------------------------
    // Bathroom validation
    // --------------------------------

    if (listing.bathrooms.has_value())
    {
        if (*listing.bathrooms < 0)
        {
            issues.push_back({
                "bathrooms",
                "Bathroom count cannot be negative"
            });
        }

        if (*listing.bathrooms > 4)
        {
            issues.push_back({
                "bathrooms",
                "Bathroom count is unusually high"
            });
        }
    }


    // --------------------------------
    // Parking validation
    // --------------------------------

    if (listing.parking.has_value())
    {
        if (*listing.parking < 0)
        {
            issues.push_back({
                "parking",
                "Parking count cannot be negative"
            });
        }

        if (*listing.parking > 5)
        {
            issues.push_back({
                "parking",
                "Parking count is unusually high"
            });
        }
    }


    // --------------------------------
    // Size validation
    // --------------------------------

    if (listing.sizeSqft.has_value())
    {
        if (*listing.sizeSqft < 100)
        {
            issues.push_back({
                "sizeSqft",
                "Listing size is suspiciously small"
            });
        }

        if (*listing.sizeSqft > 10000)
        {
            issues.push_back({
                "sizeSqft",
                "Listing size is suspiciously large"
            });
        }
    }


    return issues;
}