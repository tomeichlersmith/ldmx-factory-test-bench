#pragma once

#include <memory>

#include "Factory.h"

class LibraryEntry {
 public:
  DeclareFactory(LibraryEntry, std::shared_ptr<LibraryEntry>);
  virtual ~LibraryEntry() = default;
  virtual std::string name() = 0;
 protected:
  std::string library_name();
};

#define DECLARE_LIBRARYENTRY(CLASS) \
  namespace { \
    auto v = ::LibraryEntry::Factory::get().declare<CLASS>(); \
  }

