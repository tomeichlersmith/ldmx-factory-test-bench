#pragma once

#include <memory>

#include "Factory.h"

class LibraryEntry {
 public:
  using Factory = ::Factory<LibraryEntry, std::shared_ptr<LibraryEntry>>;
  virtual ~LibraryEntry() = default;
  virtual std::string name() = 0;
  static Factory& factory();
 private:
  static Factory the_factory_;
};

#define DECLARE_LIBRARYENTRY(CLASS) \
  namespace { \
    auto v = ::LibraryEntry::factory().declare<CLASS>(); \
  }
