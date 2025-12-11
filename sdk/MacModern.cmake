# MacModern SDK CMake Helper

# Define the Retro68 toolchain file path if not set by the user/environment
if(NOT CMAKE_TOOLCHAIN_FILE)
    # Default location in the container
    set(CMAKE_TOOLCHAIN_FILE "/opt/retro68/m68k-apple-macos-toolchain.cmake")
endif()

# Calculate the SDK Root directory based on where this file is located
get_filename_component(MACMODERN_SDK_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

# Helper function to create a modern Mac App
function(mac_modern_add_app)
    set(options)
    set(oneValueArgs TARGET NAME CREATOR)
    set(multiValueArgs SRCS LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${ARG_TARGET} ${ARG_SRCS})

    # Link against MacModern SDK libraries
    # Use the calculated root path
    target_include_directories(${ARG_TARGET} PRIVATE
        ${MACMODERN_SDK_ROOT}/lib/gui
        ${MACMODERN_SDK_ROOT}/lib/net
        ${MACMODERN_SDK_ROOT}/lib/crypto
    )

    # Link Crypto and Net sources
    target_sources(${ARG_TARGET} PRIVATE
        ${MACMODERN_SDK_ROOT}/lib/gui/Application.cpp
        ${MACMODERN_SDK_ROOT}/lib/gui/Window.cpp
        ${MACMODERN_SDK_ROOT}/lib/net/OpenTransportSocket.cpp
        ${MACMODERN_SDK_ROOT}/lib/crypto/SecureSocket.cpp
        ${MACMODERN_SDK_ROOT}/lib/crypto/monocypher.c
    )

    # Retro68 Application Bundle configuration
    set_target_properties(${ARG_TARGET} PROPERTIES
        OUTPUT_NAME "${ARG_NAME}"
        macexport_CREATOR "${ARG_CREATOR}"
        macexport_TYPE "APPL"
        macexport_SIZE 1024 # KB of heap
    )

    # Create the resource fork automatically
    # This uses Retro68's automatic resource compilation
endfunction()
