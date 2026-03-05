cmake_minimum_required(VERSION 3.16)

project(QtBaseStation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# VTK
find_package(VTK REQUIRED COMPONENTS
  CommonCore
  CommonDataModel
  FiltersSources
  InteractionStyle
  RenderingCore
  RenderingOpenGL2
  GUISupportQt
)

# ---- Sources (IMPORTANT: from SOURCE dir, not build dir) ----
set(APP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Qt6 executable
qt_add_executable(QtBaseStation
  ${APP_SOURCES}
)

# Link Qt
target_link_libraries(QtBaseStation PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Link VTK
target_link_libraries(QtBaseStation PRIVATE ${VTK_LIBRARIES})

# VTK module auto-init (needed on Windows)
vtk_module_autoinit(
  TARGETS QtBaseStation
  MODULES ${VTK_LIBRARIES}
)