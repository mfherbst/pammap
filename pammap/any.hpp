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
#include <any>

namespace pammap {

struct any : public std::any {
  using std::any::any;
};

using std::any_cast;
using std::bad_any_cast;

#if 0  // if not C++17
class any {
  void emplace();
  void reset();
  void swap();
  bool has_value();
  void type();
};

// TODO helper exception
class bad_any_cast;

template <typename T>
T any_cast(const Any& operand) {
  // TODO extract copy of inner object.
}

#endif

}  // namespace pammap
