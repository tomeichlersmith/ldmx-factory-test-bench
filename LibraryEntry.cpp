#include "LibraryEntry.h"

::LibraryEntry::Factory the_factory_
    __attribute((init_priority(500)));

::LibraryEntry::Factory& LibraryEntry::factory() {
  return the_factory_;
}
