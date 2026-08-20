cmake_minimum_required(VERSION 3.20)

project(RentalDashboard LANGUAGES CXX)

add_executable(rental_dashboard
    src/main.cpp
    src/analytics.cpp
    src/csv_loader.cpp
)

target_include_directories(rental_dashboard
    PRIVATE
    include
)

target_compile_features(rental_dashboard
    PRIVATE
    cxx_std_20
)