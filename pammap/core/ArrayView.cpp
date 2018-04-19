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

#include "ArrayView.hpp"
#include "exceptions.hpp"
#include <functional>
#include <iostream>
#include <numeric>

namespace pammap {

ArrayViewBase::ArrayViewBase(std::vector<size_t> shape_, std::vector<size_t> strides_)
      : m_shape(shape_), m_strides(strides_) {
  pammap_throw(shape_.size() == strides_.size(), ValueError,
               "Size of shape vector (== " + std::to_string(shape_.size()) +
                     ") does not agree with the size of the strides vector "
                     "(== " +
                     std::to_string(strides_.size()) + ").");

  m_size =
        std::accumulate(m_shape.begin(), m_shape.end(), 1ul, std::multiplies<size_t>{});
}

void ArrayViewBase::reset_base(void* base, BASE_KIND base_kind) {
  m_base      = base;
  m_base_kind = base_kind;
}

void ArrayViewBase::reset_base() {
  m_base      = nullptr;
  m_base_kind = NONE;
}

bool ArrayViewBase::is_fortran_contiguous() const {
  size_t acc = 1;
  for (size_t i = 0; i < ndim(); ++i) {
    if (m_strides[i] != acc) return false;
    acc *= m_shape[i];
  }
  return true;
}

bool ArrayViewBase::is_c_contiguous() const {
  size_t acc = 1;
  for (size_t i = 0; i < ndim(); ++i) {
    if (m_strides[ndim() - i - 1] != acc) return false;
    acc *= m_shape[ndim() - i - 1];
  }
  return true;
}

}  // namespace pammap

#include "ArrayView.instantiation.hxx"
