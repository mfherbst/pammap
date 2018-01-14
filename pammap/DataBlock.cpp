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

#include "DataBlock.hpp"
#include "krims/ExceptionSystem.hh"

namespace pammap {

namespace {
size_t product(const std::vector<size_t>& shape) {
  size_t acc = 1;
  for (auto& s : shape) acc *= s;
  return acc;
}
}  // namespace

template <typename T>
DataBlock<T>::DataBlock(std::initializer_list<T> data_)
      : std::vector<T>(data_),
        shape{data_.size()},
        memory_layout{MemoryLayout::RowMajor} {}

template <typename T>
DataBlock<T>::DataBlock(std::vector<T> data_)
      : DataBlock(data_, {data_.size()}, MemoryLayout::RowMajor) {}

template <typename T>
DataBlock<T>::DataBlock(std::vector<T> data_, std::vector<size_t> shape_,
                        MemoryLayout memory_layout_)
      : std::vector<T>(data_), shape(shape_), memory_layout(memory_layout_) {
  assert_size(product(shape), this->size());
}

template <typename T>
DataBlock<T>::DataBlock(T* data_, std::vector<size_t> shape_, MemoryLayout memory_layout_)
      : std::vector<T>(data_, data_ + product(shape_)),
        shape(shape_),
        memory_layout(memory_layout_) {}

// TODO explicit instantiation

}  // namespace pammap
