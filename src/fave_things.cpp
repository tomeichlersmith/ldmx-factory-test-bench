#include <dlfcn.h>
#include <iostream>
#include <string>

#include "read.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << argv[0] << " libpath libentry" << std::endl;
    return -1;
  }
  std::string full_cpp_name{argv[2]};
  std::cout << "manually loading library" << std::endl;
  void* handle = dlopen(argv[1], RTLD_NOW);
  if (handle == nullptr) {
    std::cerr << "Failed to load '" << argv[1] << "': " << dlerror() << std::endl;
    return 2;
  }
  return read(full_cpp_name);
}
