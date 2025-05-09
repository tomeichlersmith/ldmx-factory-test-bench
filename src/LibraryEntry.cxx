#include "LibraryEntry.h"

std::string LibraryEntry::library_name() {
  return "MyLibrary";
}

DefineFactory(LibraryEntry);
