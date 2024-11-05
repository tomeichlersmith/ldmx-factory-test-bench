
#include "LibraryEntry.h"
#include <iostream>
#include <string>

void run(std::string full_cpp_name) {
  try {
    auto entry_ptr{LibraryEntry::Factory::get().make(full_cpp_name)};
    std::cout << entry_ptr->name() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
}
