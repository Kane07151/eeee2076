cmake_minimum_required(VERSION 3.16)

project(QtBaseStation)

set(CMAKE_CXX_STANDARD 17)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# VTK
find_package(VTK REQUIRED)

qt_standard_project_setup()

qt_add_executable(QtBaseStation
    main.cpp
)

target_link_libraries(QtBaseStation
    PRIVATE
    Qt6::Widgets
    ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS QtBaseStation
    MODULES ${VTK_LIBRARIES}
)