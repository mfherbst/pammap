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

#include "PamMap.hpp"
#include "krims/ExceptionSystem.hh"
#include <vector>

namespace pammap {
namespace {
/** Return an iterator which points to the first key-value pair where the
 * key begins with the provided string ``start``.
 *
 * Together with map_starting_keys_end this allows to iterate over a range
 * of values in the map, where the keys start with ``start``.
 */
template <typename Map>
static auto starting_keys_begin(Map& map, const std::string& start)
      -> decltype(std::begin(map)) {
  return map.lower_bound(start);
}

/** Return an iterator which points to the first key-value pair where the
 * key does not begin with the with the provided string ``start``.
 *
 * Together with map_starting_keys_begin this allows to iterate over a range
 * of values in the map, where the keys start with ``start``.
 */
template <typename Map>
auto starting_keys_end(Map& map, const std::string& start) -> decltype(std::end(map)) {
  // If start is empty, then we iterate over the full map:
  if (start.length() == 0) return std::end(map);

  // Seek to the first key-value pair which is no longer part
  // of the range we care about, i.e. which does not start with the
  // provided start string.
  auto it = starting_keys_begin(map, start);
  for (; it != std::end(map); ++it) {
    if (0 != it->first.compare(0, start.length(), start)) break;
  }
  return it;
}
}  // namespace

PamMap& PamMap::operator=(PamMap other) {
  m_location      = std::move(other.m_location);
  m_container_ptr = std::move(other.m_container_ptr);
  return *this;
}

PamMap::PamMap(const PamMap& other) : PamMap() {
  if (other.m_location == std::string("")) {
    // We are root, copy everything
    m_container_ptr = std::make_shared<map_type>(*other.m_container_ptr);
  } else {
    update(other);
  }
}

template <typename T>
T& PamMap::at(const std::string& key, T& default_value) {
  auto itkey = m_container_ptr->find(make_full_key(key));
  if (itkey == std::end(*m_container_ptr)) {
    return default_value;
  } else {
    return any_cast<T&>(itkey->second);
  }
}

template <typename T>
const T& PamMap::at(const std::string& key, const T& default_value) const {
  auto itkey = m_container_ptr->find(make_full_key(key));
  if (itkey == std::end(*m_container_ptr)) {
    return default_value;
  } else {
    return any_cast<const T&>(itkey->second);
  }
}

void PamMap::update(std::initializer_list<entry_type> il) {
  // Make each key a full path key and append/modify entry in map
  for (entry_type t : il) {
    (*m_container_ptr)[make_full_key(t.first)] = std::move(t.second);
  }
}

void PamMap::clear() {
  if (m_location == std::string("")) {
    // We are root, clear everything
    m_container_ptr->clear();
  } else {
    // Clear only our stuff
    erase(begin(), end());
  }
}

void PamMap::update(const std::string& key, const PamMap& other) {
  for (auto it = other.begin(); it != other.end(); ++it) {
    // The iterator truncates the other key relative to the builtin
    // location of other for us. We then make it full for our location
    // and update.
    (*m_container_ptr)[make_full_key(key + "/" + it->key())] = it->value_raw();
  }
}

void PamMap::update(const std::string& key, PamMap&& other) {
  for (auto it = other.begin(); it != other.end(); ++it) {
    // The iterator truncates the other key relative to the builtin
    // location of other for us. We then make it full for our location
    // and update.
    (*m_container_ptr)[make_full_key(key + "/" + it->key())] = std::move(it->value_raw());
  }
}

std::string PamMap::make_full_key(const std::string& key) const {
  assert_internal(m_location[0] == '/' || m_location.length() == 0);
  assert_internal(m_location.back() != '/');

  // Make a stack out of the key:
  std::vector<std::string> pathparts;

  // start gives the location after the last '/',
  // ie where the current part of the key path begins and end gives
  // the location of the current '/', i.e. the past-the-end index
  // of the current path part.
  for (size_t start = 0; start < key.size(); ++start) {
    // Past-the-end of the current path part:
    const size_t end = key.find('/', start);

    // Empty path part (i.e. something like '//' is encountered:
    if (start == end) continue;

    // Extract the part we deal with in this iteration:
    std::string part = key.substr(start, end - start);

    // Update start for next iteration:
    start += part.length();

    if (part == ".") {
      // Ignore "." path part (does nothing)
      continue;
    } else if (part == "..") {
      // If ".." path part, then pop the most recently added path part if
      // any.
      if (!pathparts.empty()) pathparts.pop_back();
    } else {
      pathparts.push_back(std::move(part));
    }
  }

  std::string res{m_location};
  for (const auto& part : pathparts) {
    res += "/" + part;
  }

  assert_internal(res.length() == 0 || res.back() != '/');
  assert_internal(res.length() == 0 || res[0] == '/');

  return res;
}

typename PamMap::iterator PamMap::begin(const std::string& path) {
  // Obtain iterator to the first key-value pair, which has a
  // key starting with the full path.
  //
  // (since the keys are sorted alphabetically in the map
  //  the ones which follow next must all be below our current
  //  location or already well past it.)
  const std::string path_full = make_full_key(path);
  return iterator(starting_keys_begin(*m_container_ptr, path_full), path_full);
}

typename PamMap::const_iterator PamMap::cbegin(const std::string& path) const {
  const std::string path_full = make_full_key(path);
  return const_iterator(starting_keys_begin(*m_container_ptr, path_full), path_full);
}

typename PamMap::iterator PamMap::end(const std::string& path) {
  // Obtain the first key which does no longer start with the pull path,
  // i.e. where we are done processing the subpath.
  const std::string path_full = make_full_key(path);
  return iterator(starting_keys_end(*m_container_ptr, path_full), path_full);
}

typename PamMap::const_iterator PamMap::cend(const std::string& path) const {
  const std::string path_full = make_full_key(path);
  return const_iterator(starting_keys_end(*m_container_ptr, path_full), path_full);
}

}  // namespace pammap

#include "PamMap.instantiation.hxx"
