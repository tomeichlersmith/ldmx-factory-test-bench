#pragma once

#include <memory>

#include "Factory.h"

class LibraryEntry {
 public:
  virtual ~LibraryEntry() = default;
  virtual std::string name() = 0;
  using Factory = ::Factory<LibraryEntry, std::shared_ptr<LibraryEntry>>;
};

#define DECLARE_LIBRARYENTRY(CLASS) \
  namespace { \
    auto v = ::LibraryEntry::Factory::get().declare<CLASS>(); \
  }
