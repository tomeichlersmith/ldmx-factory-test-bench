#include "LibraryEntry.h"

#include <dlfcn.h>

int main(int argc, char* argv[]) {
#ifdef SEPARATE_LIB
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
#else
  if (argc != 2) {
    std::cout << argv[0] << " libentry" << std::endl;
    return 0;
  }
  std::string full_cpp_name{argv[1]};
#endif
  try {
    auto entry_ptr{LibraryEntry::Factory::get().make(full_cpp_name)};
    std::cout << entry_ptr->name() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
}
