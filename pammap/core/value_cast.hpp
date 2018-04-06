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
#include "PamMapValue.hxx"
#include "demangle.hpp"
#include "exceptions.hpp"

namespace pammap {

namespace detail {
inline void throw_value_cast_type_error(const PamMapValue& operand,
                                        const std::type_info& reqtype) {
  pammap_throw(false, TypeError,
               "Key '" + key + "' points to a value of type '" + operand.type_name() +
                     "', which cannot be converted to the requested type '" +
                     demangle(reqtype) + "'.");
}
}  // namespace detail

//@{
/** Perform type-safe access into object contained in PamMapValue.
 *
 * If this access is not possible throws a TypeError
 */
template <typename ValueType>
ValueType value_cast(const std::string& key, const PamMapValue& operand) {
  try {
    return any_cast<ValueType>(operand);
  } catch (bad_any_cast& e) {
    throw_value_cast_type_error(operand, typeid(ValueType));
  }
}

template <typename ValueType>
ValueType value_cast(const std::string& key, PamMapValue& operand) {
  try {
    return any_cast<ValueType>(operand);
  } catch (bad_any_cast& e) {
    throw_value_cast_type_error(operand, typeid(ValueType));
  }
}

template <typename ValueType>
ValueType value_cast(const std::string& key, PamMapValue&& operand) {
  try {
    return any_cast<ValueType>(operand);
  } catch (bad_any_cast& e) {
    throw_value_cast_type_error(operand, typeid(ValueType));
  }
}
//@}

}  // namespace pammap
