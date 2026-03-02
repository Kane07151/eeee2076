cmake_minimum_required(VERSION 3.16)
project(Worksheet5 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build executable
add_executable(calc
    calc.cpp
    adder.cpp
)

target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Install rules (this is what CPack uses)
install(TARGETS calc
    RUNTIME DESTINATION bin
)

# CPack settings
set(CPACK_PACKAGE_NAME "Worksheet5")
set(CPACK_PACKAGE_VENDOR "EEEE2076")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

# On Windows, prefer NSIS if available; otherwise ZIP will still work
set(CPACK_GENERATOR "NSIS;ZIP")

include(CPack)
