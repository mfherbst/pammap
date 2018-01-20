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
#include "PamMapValue.hpp"
#include <string>

namespace pammap {

/** Accessor to a GenMap object. Can be used to retrieve the key or the value
 *  or the typename of the value */
template <bool Const = true>
class PamMapAccessor {
 public:
  /** Return the key of the key/value pair the accessor holds */
  const std::string& key() const { return m_key; }

  /** Return the type name of the value object referred to by the key, which
   * is held in this accessor.
   *
   * The type_name is only sensible in DEBUG mode.
   */
  std::string type_name() const { return m_value.type_name(); }

  /** Return the value of the key/value pair the accessor holds (Const
   *version).
   *
   * Raises an exception if the value object referred to by this key
   * cannot be converted to this value type.
   **/
  template <typename T>
  const T& value() const {
    return any_cast<const T&>(m_value);
  }

  /** Return a reference to the raw value object the accessor holds. (Const
   *version)
   *
   * \note This is an advanced method.
   *       Use only if you know what you are doing.
   **/
  const PamMapValue& value_raw() const { return m_value; }

  /** Construct an accessor */
  PamMapAccessor(const std::string key, const PamMapValue& value)
        : m_key(key), m_value(value) {}

 private:
  const std::string m_key;
  const PamMapValue& m_value;
};

template <>
class PamMapAccessor<false> : public PamMapAccessor<true> {
  // Use the const version as the base type:
  typedef PamMapAccessor<true> base_type;

 public:
  using base_type::value;
  using base_type::value_raw;

  /** Return the value of the key/value pair the accessor holds.
   *
   * Raises an exception if the value object referred to by this key
   * cannot be converted to this value type.
   **/
  template <typename T>
  T& value() {
    return any_cast<T&>(m_value);
  }

  /** Return a reference to the raw value object the accessor holds.
   *
   * \note This is an advanced method.
   *        Use only if you know what you are doing.
   **/
  PamMapValue& value_raw() { return m_value; }

  /** Construct an accessor */
  PamMapAccessor(const std::string key, PamMapValue& value)
        : base_type(key, value), m_value(value) {}

 private:
  PamMapValue& m_value;
};

}  // namespace pammap
