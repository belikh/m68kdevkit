cmake_minimum_required(VERSION 3.12)
project(LLMClient CXX)

include(../../sdk/MacModern.cmake)

mac_modern_add_app(
    TARGET llm_client
    NAME "LLM Client"
    CREATOR "LLMC"
    SRCS main.cpp
)
