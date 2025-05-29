#pragma once

#include <memory>

#include "Factory.h"

class LibraryEntry {
 public:
  DECLARE_FACTORY(LibraryEntry, std::shared_ptr<LibraryEntry>);
  virtual ~LibraryEntry() = default;
  virtual std::string name() = 0;
 protected:
  std::string library_name();
};
