#include "LibraryEntry.h"

class Album : public LibraryEntry {
 public:
  std::string name() override {
    return "Kind of Blue";
  }
};

DECLARE_LIBRARYENTRY(Album);
