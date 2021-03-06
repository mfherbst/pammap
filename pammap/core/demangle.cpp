//
// Copyright (C) 2018 by Michael F. Herbst and contributors
//
// This file is part of pammap.
//
// pammap is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// pammap is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with pammap. If not, see <http://www.gnu.org/licenses/>.
//

#include "demangle.hpp"
#include <cstdlib>
#include <cstring>
#include <cxxabi.h>

namespace pammap {

std::string demangle(const char* mangled) {
  int status;
  char* demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);

  if (status == 0) {
    std::string ret(demangled);
    free(demangled);  // NOLINT
    return ret;
  }
  return std::string(mangled);
}

}  // namespace pammap
