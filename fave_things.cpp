#include "LibraryEntry.h"

#include <dlfcn.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << argv[0] << " libpath libentry" << std::endl;
    return 0;
  }
  std::cout << "manually loading library" << std::endl;
  if (dlopen(argv[1], RTLD_NOW) == nullptr) {
    std::cerr << "Failed to load '" << argv[1] << "': " << dlerror() << std::endl;
    return 1;
  }
  std::cout << "done loading library" << std::endl;
  std::string full_cpp_name{argv[2]};
  try {
    auto entry_ptr{LibraryEntry::factory().make(full_cpp_name)};
    std::cout << entry_ptr->name() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
}
