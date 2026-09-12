cmake_minimum_required(VERSION 3.20)

project(RentalDashboard LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use C++20 for this project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Drogon installed by Homebrew
find_package(Drogon CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# ---------------------------------------------------------
# Reusable rental logic
# ---------------------------------------------------------

add_library(rental_core STATIC
    src/analytics.cpp
    src/csv_loader.cpp
    src/csv_utils.cpp
    src/price_history.cpp
    src/validation.cpp
)

target_include_directories(rental_core
    PUBLIC
    include
)


# ---------------------------------------------------------
# Terminal application
# ---------------------------------------------------------

add_executable(rental_dashboard
    src/main.cpp
)

target_link_libraries(rental_dashboard
    PRIVATE
    rental_core
)


# ---------------------------------------------------------
# Web application
# ---------------------------------------------------------

add_executable(rental_web
    src/web_main.cpp
)

target_link_libraries(rental_web
    PRIVATE
    rental_core
    Drogon::Drogon
)

add_executable(rental_alert_checker
    src/alert_main.cpp
    src/discord_alert.cpp
    src/alert_state.cpp
)

target_link_libraries(rental_alert_checker
    PRIVATE
    rental_core
    CURL::libcurl
    JsonCpp::JsonCpp
)