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
#include "PamMapAccessor.hpp"
#include "exceptions.hpp"
#include <iterator>
#include <map>
#include <memory>
#include <type_traits>

namespace pammap {

template <bool Const>
class PamMapIterator
      : std::iterator<std::bidirectional_iterator_tag, PamMapAccessor<Const>> {
 public:
  /** The map type we assume */
  typedef typename std::map<std::string, PamMapValue> map_type;

  /** The resulting inner iterator type */
  typedef typename std::conditional<Const, typename map_type::const_iterator,
                                    typename map_type::iterator>::type inner_iter_type;

  /** Dereference PamMap iterator */
  PamMapAccessor<Const>& operator*() const { return *operator->(); }

  /** Obtain pointer to PamMap accessor */
  PamMapAccessor<Const>* operator->() const;

  /** Prefix increment to the next key */
  PamMapIterator& operator++() {
    ++m_iter;
    m_acc_ptr.reset();  // Reset cache
    return *this;
  }

  /** Postfix increment to the next key */
  PamMapIterator operator++(int) {
    PamMapIterator copy(*this);
    this->operator++();
    return copy;
  }

  /** Prefix decrement to the next key */
  PamMapIterator& operator--() {
    --m_iter;
    m_acc_ptr.reset();  // Reset cache
    return *this;
  }

  /** Postfix decrement to the next key */
  PamMapIterator operator--(int) {
    PamMapIterator copy(*this);
    this->operator--();
    return copy;
  }

  bool operator==(const PamMapIterator& other) const { return m_iter == other.m_iter; }
  bool operator!=(const PamMapIterator& other) const { return m_iter != other.m_iter; }

  /** Explicit conversion to the inner iterator type */
  explicit operator inner_iter_type() { return m_iter; }

  PamMapIterator(inner_iter_type iter, std::string location)
        : m_acc_ptr(nullptr), m_iter(iter), m_location(std::move(location)) {}

  PamMapIterator() : m_acc_ptr(nullptr), m_iter(), m_location() {}

 private:
  /** Undo the operation of PamMap::make_full_key, i.e. strip off the
   * first location part and get a relative path to it*/
  std::string strip_location_prefix(const std::string& key) const;

  /** Cache for the accessor of the current value.
   *  A stored nullptr implies that the accessor needs to rebuild
   *  before using it.*/
  mutable std::shared_ptr<PamMapAccessor<Const>> m_acc_ptr;

  /** Iterator to the current key,value pair */
  inner_iter_type m_iter;

  /** Subtree location we iterate over */
  std::string m_location;
};

//
// -----------------------------------------------
//

template <bool Const>
PamMapAccessor<Const>* PamMapIterator<Const>::operator->() const {
  if (m_acc_ptr == nullptr) {
    // Generate accessor for current state
    const std::string key_stripped = strip_location_prefix(m_iter->first);
    m_acc_ptr = std::make_shared<PamMapAccessor<Const>>(key_stripped, m_iter->second);
  }

  return m_acc_ptr.get();
}

template <bool Const>
std::string PamMapIterator<Const>::strip_location_prefix(const std::string& key) const {
  // The first part needs to be exactly the location:
  pammap_assert(key.size() >= m_location.size());
  pammap_assert(0 == key.compare(0, m_location.size(), m_location));

  if (key.size() <= m_location.size()) {
    return "/";
  } else {
    std::string res = key.substr(m_location.size());
    pammap_assert(res[0] == '/' || res.length() == 0);
    pammap_assert(res.back() != '/');
    return res;
  }
}

}  // namespace pammap
