cmake_minimum_required(VERSION 3.12)
project(HelloWorld CXX)

include(../../sdk/MacModern.cmake)

mac_modern_add_app(
    TARGET hello_world
    NAME "Hello World"
    CREATOR "HELO"
    SRCS main.cpp
)
