cmake_minimum_required(VERSION 3.12)
project(lto-dev LANGUAGES CXX)

option(SEPARATE_LIB "compile library into separate so" ON)
option(ENABLE_LTO "enable lto" OFF)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic)

if (NOT SEPARATE_LIB)
  add_executable(fave-things
    fave_things.cpp
    Book.cpp
    Album.cpp
    Podcast.cpp
  )
  if (ENABLE_LTO)
    set_property(TARGET fave-things PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
else()
  add_library(Library SHARED Book.cpp Album.cpp Podcast.cpp)
  set_property(TARGET Library PROPERTY ENABLE_EXPORTS TRUE)
  add_executable(fave-things fave_things.cpp)
  target_compile_definitions(fave-things PRIVATE SEPARATE_LIB)
  set_property(TARGET fave-things PROPERTY ENABLE_EXPORTS TRUE)
  if (ENABLE_LTO)
    set_property(TARGET Library PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET fave-things PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
endif()

