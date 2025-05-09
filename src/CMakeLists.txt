cmake_minimum_required(VERSION 3.12)
project(factory-test-bench LANGUAGES CXX)

option(ENABLE_LTO "enable lto" OFF)
option(ENABLE_SYMB_EXPORTS "inform linker that there are symbols exported from one library that should be used by another" OFF)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic)
add_library(Library SHARED Book.cpp)
add_library(read SHARED read.cpp LibraryEntry.cxx)
target_link_libraries(read PUBLIC Library)
add_executable(fave-things fave_things.cpp)
target_link_libraries(fave-things PRIVATE read)
if (ENABLE_LTO)
  set_target_properties(
    fave-things read Library
    PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()
if (ENABLE_SYMB_EXPORTS)
  set_target_properties(
    fave-things read Library
    PROPERTIES
    ENABLE_EXPORTS TRUE
  )
endif()

