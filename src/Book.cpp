#include "LibraryEntry.h"

class Book : public LibraryEntry {
 public:
  std::string name() override {
    return "Poverty, By America";
  }
};

DECLARE_LIBRARYENTRY(Book);
