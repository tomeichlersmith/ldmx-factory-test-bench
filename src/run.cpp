
#include "LibraryEntry.h"
#include <iostream>
#include <string>

int run(std::string full_cpp_name) {
  try {
    auto entry_ptr{LibraryEntry::Factory::get().make(full_cpp_name)};
    std::cout << entry_ptr->name() << std::endl;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}
