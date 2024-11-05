#include <dlfcn.h>
#include <iostream>
#include <string>

#ifdef SEPARATE_LIB
using run_type = void(*)(std::string);
#else
#include "LibraryEntry.h"
#endif

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << argv[0] << " libpath libentry" << std::endl;
    return 0;
  }
  std::string full_cpp_name{argv[2]};
#ifdef SEPARATE_LIB
  std::cout << "manually loading library" << std::endl;
  void* handle = dlopen(argv[1], RTLD_NOW);
  if (handle == nullptr) {
    std::cerr << "Failed to load '" << argv[1] << "': " << dlerror() << std::endl;
    return 1;
  }
  void* run_func_handle = dlsym(handle, "run");
  if (run_func_handle == nullptr) {
    std::cerr << "Failed to load symbol 'run' from '" << argv[1] << "': " << dlerror() << std::endl;
    return 1;
  }
  // cast void* handle to function type it should be
  run_type run;
  *(void**)(&run) = run_func_handle;
  std::cout << "done loading library" << std::endl;
  run(full_cpp_name);
#else
  std::cout << "library compiled into executable" << std::endl;
  run(full_cpp_name);
#endif
  return 0;
}
