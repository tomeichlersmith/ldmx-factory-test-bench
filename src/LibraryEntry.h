#pragma once

#include <memory>

#include "Factory.h"

class LibraryEntry {
 public:
  using Factory = ::Factory<LibraryEntry, std::shared_ptr<LibraryEntry>>;
  virtual ~LibraryEntry() = default;
  virtual std::string name() = 0;
};

#define DECLARE_LIBRARYENTRY(CLASS) \
  namespace { \
    auto v = ::LibraryEntry::Factory::get().declare<CLASS>(); \
  }

