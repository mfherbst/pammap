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

#pragma once
#include "DataBlock.hpp"
#include "any.hpp"
#include "demangle.hpp"

namespace pammap {

/** \brief Class to contain an entry value in a PamMap, i.e. the thing the
 *  key string actually points to. Essentially a slightly specialised pammap::any*/
class PamMapValue : public any {
 public:
  using any::any;

  /** \brief Make an PamMapValue out of a const char*.
   * This behaves like the equivalent GenMapValue of a  std::string */
  PamMapValue(const char* s) : PamMapValue(std::string(s)) {}

  /** \brief Make an PamMapValue out of a vector */
  template <typename T>
  PamMapValue(const std::vector<T>&) : PamMapValue() {
    static_assert(true,
                  "Cannot assign a list/array of values as a std::vector. "
                  "Use the low-level DataBlock<T> object for this.");
  }

  template <typename T>
  PamMapValue(const std::initializer_list<T> il) : PamMapValue(DataBlock(il)) {}

  /** Return the demangled typename of the type of the internal object. */
  std::string type_name() const { return demangle(any::type()); }
};

// TODO Custom any_cast, which catches the std::bad_any_cast
//      and throws a custom thing instead?
//
//      e.g. ExcWrongTypeRequested

}  // namespace pammap
