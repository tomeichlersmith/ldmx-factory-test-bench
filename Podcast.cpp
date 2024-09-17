#include "LibraryEntry.h"

class Podcast : public LibraryEntry {
 public:
  std::string name() override {
    return "Rosecast";
  }
};

DECLARE_LIBRARYENTRY(Podcast);
