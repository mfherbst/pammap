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
#include <cstddef>
#include <ostream>
#include <string>
#include <tuple>

namespace pammap {

static constexpr size_t Auto = static_cast<size_t>(-1);

/** Class defining slicing along a particular axis. */
struct Slice {
  ///@{
  /** Construct a slice
   *
   * \param begin_     Index of the first element
   * \param end_       Index *past* the last element
   * \param step_      Steps in which indices are to be increased.
   */
  Slice(size_t begin, size_t end, ptrdiff_t step);
  Slice(size_t begin, size_t end) : begin(begin), end(end), step(1) {}
  Slice(size_t begin) : begin(begin), end(begin + 1), step(1) {}
  Slice() : begin(Auto), end(Auto), step(1) {}
  ///@}

  /** Assuming an extent of length len, compute the start and stop
   *  indices as well as the stride length of the slice described by
   *  this object.
   *
   *  The function returns the tuple (begin, end, stride),
   *  where begin is the first index to be considered and end
   *  is the index past the last to be considered.
   *
   *  In Debug builds invalid index ranges cause an IndexError
   *  to be thrown.
   */
  std::tuple<ptrdiff_t, ptrdiff_t, ptrdiff_t> indices(size_t len) const;

  size_t begin;
  size_t end;
  ptrdiff_t step;
};

/** Special Slice, which selects all members along a particular axis */
const Slice All{Auto, Auto, 1};

std::ostream& operator<<(std::ostream& o, const Slice& s);

/** Transform slice into a human readable string */
std::string to_string(const Slice& s);

}  // namespace pammap
