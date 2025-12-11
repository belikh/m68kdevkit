cmake_minimum_required(VERSION 3.12)
project(AnimationDemo CXX)

include(../../sdk/MacModern.cmake)

mac_modern_add_app(
    TARGET animation
    NAME "Animation Demo"
    CREATOR "ANIM"
    SRCS main.cpp
)
