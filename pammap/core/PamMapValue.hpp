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
#include "IsSupportedType.hxx"
#include "any.hpp"
#include "demangle.hpp"
#include "typedefs.hxx"

namespace pammap {

/** \brief Class to contain an entry value in a PamMap, i.e. the thing the
 *  key string actually points to. Essentially a slightly specialised pammap::any*/
class PamMapValue : public any {
 public:
  PamMapValue() = default;

  // TODO Have it explicit?

  template <typename ValueType,
            typename = typename std::enable_if<IsSupportedType<ValueType>::value>::type>
  PamMapValue(ValueType&& value) : any(std::forward<ValueType>(value)) {
    static_assert(IsSupportedType<ValueType>::value,
                  "This value type is not supported by PamMap.");
  }

  /** \brief Make an PamMapValue out of a const char*.
   * This behaves like the equivalent GenMapValue of a  std::string */
  PamMapValue(const char* s) : PamMapValue(std::string(s)) {}

  /** \brief Make an PamMapValue out of a vector */
  template <typename T>
  PamMapValue(std::vector<T>) : PamMapValue() {
    static_assert(true,
                  "Cannot assign a list/array of values as a std::vector. "
                  "Use the low-level DataBlock<T> object for this.");
  }

  /** \brief Make an PamMapValue out of an unsigned integer type */
  template <typename T,
            typename = typename std::enable_if<std::is_unsigned<T>::value>::type>
  PamMapValue(T) : PamMapValue() {
    static_assert(true,
                  "Unsigned integer types are not supported with a PamMap. Use a signed "
                  "integer type instead.");
  }

  /** Explicit instantiation of integer */
  PamMapValue(Integer i) : any(i) {}

  /** \brief Make a PamMapValue out of an initialiser list */
  template <typename T>
  PamMapValue(const std::initializer_list<T> il) : PamMapValue(DataBlock(il)) {}

  /** Return the demangled typename of the type of the internal object. */
  std::string type_name() const { return demangle(any::type()); }

  //
  // Special treatment of the default integer type int
  // Such that simple assignments just work.
  //
  /** \brief Make an PamMapValue out of an int */
  PamMapValue(int i) : PamMapValue(static_cast<Integer>(i)) {}

  /** \brief Make a PamMapValue out of an initialiser list of int */
  PamMapValue(std::initializer_list<int> il)
        : PamMapValue(DataBlock<Integer>(std::vector<Integer>(il.size()))) {
    auto& data = any_cast<DataBlock<Integer>&>(*this);
    std::copy(il.begin(), il.end(), data.begin());
  }
};

// TODO Custom any_cast, which catches the std::bad_any_cast
//      and throws a custom thing instead?
//
//      e.g. ExcWrongTypeRequested

}  // namespace pammap
