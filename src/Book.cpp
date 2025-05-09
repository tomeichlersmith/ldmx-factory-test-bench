#include "LibraryEntry.h"

class Book : public LibraryEntry {
 public:
  std::string name() override {
    return "Poverty, By America from "+library_name();
  }
};

DECLARE_LIBRARYENTRY(Book);
