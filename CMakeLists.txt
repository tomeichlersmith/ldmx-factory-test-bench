cmake_minimum_required(VERSION 3.12)
project(lto-dev)
add_library(Library SHARED Book.cpp Album.cpp Podcast.cpp)
add_executable(fave-things fave_things.cpp LibraryEntry.cpp)

option(ENABLE_LTO "enable lto" OFF)
if (ENABLE_LTO)
  set_property(TARGET Library PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  set_property(TARGET fave-things PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

