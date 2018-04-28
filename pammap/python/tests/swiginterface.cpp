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

#include "swiginterface.hpp"
#include <iterator>

namespace swiginterface_test {

Integer num_entries(pammap::PamMap map) {
  Integer i = 0;
  for (auto it = map.begin(); it != map.end(); ++it) ++i;
  return i;
}

Float max_value_float(pammap::ArrayView<Float> array) {
  if (array.size() == 0) return 0.;

  Float max = array[0];
  for (size_t i = 0; i < array.size(); ++i) {
    max = std::max(max, array[i]);
  }
  return max;
}

Integer max_value_integer(pammap::ArrayView<Integer> array) {
  if (array.size() == 0) return 0;

  Integer max = array[0];
  for (size_t i = 0; i < array.size(); ++i) {
    max = std::max(max, array[i]);
  }
  return max;
}

}  // namespace swiginterface_test
