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

#include "Slice.hpp"
#include "exceptions.hpp"
#include <sstream>

namespace pammap {

Slice::Slice(size_t begin_, size_t end_, ptrdiff_t step_)
      : begin(begin_), end(end_), step(step_) {
  pammap_throw(step != 0, ValueError, "Step cannot be zero.");
}

std::tuple<ptrdiff_t, ptrdiff_t, ptrdiff_t> Slice::indices(size_t len) const {
  const ptrdiff_t pastlast = [this, &len] {
    if (end != Auto) {
      return static_cast<ptrdiff_t>(end);
    } else if (step > 0) {
      return static_cast<ptrdiff_t>(len);
    } else {
      return static_cast<ptrdiff_t>(-1);
    }
  }();

  const ptrdiff_t first = [this, &len] {
    if (begin != Auto) {
      return static_cast<ptrdiff_t>(begin);
    } else if (step > 0) {
      return static_cast<ptrdiff_t>(0);
    } else {
      return static_cast<ptrdiff_t>(len) - 1;
    }
  }();

#ifndef NDEBUG
  const ptrdiff_t plen = static_cast<ptrdiff_t>(len);
  pammap_throw(pastlast <= plen || pastlast < -1, IndexError,
               "Slice " + to_string(*this) + " goes beyond the valid index range {0, " +
                     std::to_string(len) + "}.");
  pammap_throw(first < plen || first < 0, IndexError,
               "Slice " + to_string(*this) + " goes beyond the valid index range {0, " +
                     std::to_string(len) + "}.");

  pammap_throw((step < 0) || (first < pastlast), IndexError,
               "Slice " + to_string(*this) + " is empty.");
  pammap_throw((step > 0) || (first > pastlast), IndexError,
               "Slice " + to_string(*this) + " is empty.");

#endif
  return std::make_tuple(std::move(first), std::move(pastlast), step);
}

std::ostream& operator<<(std::ostream& o, const Slice& s) {
  using namespace std;
  o << to_string(s);
  return o;
}

std::string to_string(const Slice& s) {
  std::stringstream ss;
  ss << "(";
  if (s.begin == Auto) {
    ss << "Auto";
  } else {
    ss << s.begin;
  }
  ss << ", ";
  if (s.end == Auto) {
    ss << "Auto";
  } else {
    ss << s.end;
  }
  ss << ", " << s.step << ")";
  return ss.str();
}

}  // namespace pammap
