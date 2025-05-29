
#include "LibraryEntry.h"
#include <iostream>
#include <string>

int read(std::string full_cpp_name) {
  auto entry_ptr{LibraryEntry::Factory::get().make(full_cpp_name)};
  if (entry_ptr) {
    std::cout << entry_ptr.value()->name() << std::endl;
    return 0;
  } else {
    std::cerr << "Unable to create " << full_cpp_name << std::endl;
    return 1;
  }
}
