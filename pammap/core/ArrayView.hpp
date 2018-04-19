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
#include <vector>

namespace pammap {

struct ArrayViewBase {
  /** Enum to mark the kind of object the owner pointer points to */
  enum OWNER_TYPE {
    NONE  = 0,
    NUMPY = 1,
  };

  ArrayViewBase() = default;
  ArrayViewBase(std::vector<size_t> shape_, std::vector<size_t> strides_)
        : shape(shape_), strides(strides_) {}

  /** The shape in each dimension */
  std::vector<size_t> shape;

  /** The strides in each dimension */
  std::vector<size_t> strides;

  /** Some extra pointer to additional data,
   *  e.g. the original numpy array the data points to.
   */
  void* owner = nullptr;

  /** Some info about the owner pointer */
  OWNER_TYPE owner_type = NONE;
};

/** Light-weight class to view some array data. */
template <typename T>
struct ArrayView : public ArrayViewBase {
  /** The type of the ArrayView data */
  typedef T value_type;

  /** The pointer to the data */
  T* data = nullptr;

  ArrayView() = default;
  ArrayView(std::vector<T>& vector)
        : ArrayViewBase{{vector.size()}, {1}}, data{vector.data()} {}
  ArrayView(T* data_, std::vector<size_t> shape_, std::vector<size_t> strides_)
        : ArrayViewBase{shape_, strides_}, data(data_) {}
};

}  // namespace pammap
