cmake_minimum_required(VERSION 3.12)
project(HassDashboard CXX)

include(../../sdk/MacModern.cmake)

mac_modern_add_app(
    TARGET hass_dashboard
    NAME "Hass Dashboard"
    CREATOR "HASS"
    SRCS main.cpp
)
