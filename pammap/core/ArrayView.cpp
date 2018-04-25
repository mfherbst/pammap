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
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

namespace pammap {

ArrayViewBase::ArrayViewBase(const std::vector<size_t>& shape,
                             std::vector<ptrdiff_t> strides)
      : m_shape(shape), m_strides(strides) {
  pammap_throw(shape.size() == strides.size(), ValueError,
               "Size of shape vector (== " + std::to_string(shape.size()) +
                     ") does not agree with the size of the strides vector "
                     "(== " +
                     std::to_string(strides.size()) + ").");

  if (!strides.empty()) {
    m_unit = *std::min_element(
          strides.begin(), strides.end(),
          [](ptrdiff_t a, ptrdiff_t b) { return std::abs(a) < std::abs(b); });
  }

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
  ptrdiff_t acc = 1;
  for (size_t i = 0; i < ndim(); ++i) {
    if (m_strides[i] != acc) return false;
    acc *= static_cast<ptrdiff_t>(m_shape[i]);
  }
  return true;
}

bool ArrayViewBase::is_c_contiguous() const {
  ptrdiff_t acc = 1;
  for (size_t i = 0; i < ndim(); ++i) {
    if (m_strides[ndim() - i - 1] != acc) return false;
    acc *= static_cast<ptrdiff_t>(m_shape[ndim() - i - 1]);
  }
  return true;
}

template <typename T>
bool ArrayView<T>::operator==(const ArrayView& other) const {
  if (m_size != other.m_size) return false;
  if (m_shape != other.m_shape) return false;
  if (m_strides != other.m_strides) return false;
  if (m_data == other.m_data) return true;
  for (size_t i = 0; i < m_size; ++i) {
    if ((*this)[i] != other[i]) return false;
  }
  return true;
}

template <typename T>
ArrayView<T> ArrayView<T>::slice_view(std::initializer_list<Slice> idcs) const {
  pammap_throw(false, NotImplementedError, "the slice() functions have not been tested.");

  pammap_throw(idcs.size() == m_shape.size(), IndexError,
               "Number of slice objects provided to 'slice()' does not agree with number "
               "of dimensions of ArrayView.");
  const size_t ndim = m_shape.size();

#ifndef NDEBUG
  pammap_assert(m_shape.size() == m_strides.size());
#endif  // NDEBUG

  // The containers to form the new array
  T* data(m_data);
  std::vector<size_t> shape(ndim);
  std::vector<ptrdiff_t> strides(ndim);

  // Go through all slides and adjust shape and strides
  size_t d = 0;
  for (const auto& sl : idcs) {
    // obtain begin end and stride indices using the shape along
    // current dimension
    ptrdiff_t begin, end, stride;
    std::tie(begin, end, stride) = sl.indices(m_shape[d]);

    strides[d] = stride;
    data += begin * m_strides[d];
    const ptrdiff_t shd = (end - begin) / stride;
#ifndef NDEBUG
    pammap_assert(shd > 0);
    pammap_assert(static_cast<size_t>(shd) <= m_shape[d]);
#endif  // NDEBUG
    shape[d] = static_cast<size_t>(shd);

    ++d;
  }

  return ArrayView(data, std::move(shape), std::move(strides));
}

}  // namespace pammap

#include "ArrayView.instantiation.hxx"
