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
#include <pammap/core.hpp>

namespace swiginterface_test {
using pammap::Float;
using pammap::Integer;

/** Return the number of entries in a PamMap */
Integer num_entries(pammap::PamMap map);

/** Obtain the largest value of an array */
Float max_value_float(pammap::ArrayView<Float> array);

/** Obtain the largest value of an array */
Integer max_value_integer(pammap::ArrayView<Integer> array);

}  // namespace swiginterface_test
