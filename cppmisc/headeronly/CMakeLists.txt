cmake_minimum_required(VERSION 3.10)

project(calculator LANGUAGES CXX)

option(USE_HEADER_ONLY "Use header-only implementation" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

if(USE_HEADER_ONLY)
  message(STATUS "Building with header-only implementation")
  add_executable(main)
  target_sources(main PRIVATE src/main.cc src/odr.cc)
  target_include_directories(main PRIVATE include)
  target_compile_definitions(main PRIVATE CALCULATOR_HEADER_ONLY)
else()
  message(STATUS "Building with shared library")

  add_library(calculator SHARED)
  target_sources(calculator PRIVATE src/calculator.cc)
  target_include_directories(calculator PUBLIC include)

  add_executable(main)
  target_sources(main PRIVATE src/main.cc src/odr.cc)
  target_include_directories(main PRIVATE include)
  target_link_libraries(main calculator)
endif()
