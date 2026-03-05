cmake_minimum_required(VERSION 3.20)

project(CylinderExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find VTK (installed)
find_package(VTK REQUIRED)

add_executable(CylinderExample CylinderExample.cxx)

# Link against VTK
target_link_libraries(CylinderExample PRIVATE ${VTK_LIBRARIES})

# Ensure VTK modules are initialized correctly (important for VTK 9+)
vtk_module_autoinit(
  TARGETS CylinderExample
  MODULES ${VTK_LIBRARIES}
)