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
#include <array>
#include <initializer_list>
#include <vector>

namespace pammap {

enum class MemoryLayout {
  RowMajor,
  C = RowMajor,
  ColumnMajor,
  Fortran = ColumnMajor,
};

/** Simple low-level class to store a block of data */
template <typename T>
struct DataBlock : std::vector<T> {
  template <typename Input>
  explicit DataBlock(Input first, Input last)
        : std::vector<T>(first, last), shape{0}, memory_layout{MemoryLayout::RowMajor} {
    shape = std::vector<T>::size();
  }

  /** Initialise from initializer_list */
  DataBlock(std::initializer_list<T> data_);

  /** Initialise from std::vector as 1d array */
  DataBlock(std::vector<T> data_);

  /** Initialise from vector, shape and memory layout */
  DataBlock(std::vector<T> data_, std::vector<size_t> shape_,
            MemoryLayout memory_layout_ = MemoryLayout::RowMajor);

  /** Initialise by copying from a memory location and a shape,
   *  assuming a certain memory layout. */
  explicit DataBlock(T* data_, std::vector<size_t> shape_,
                     MemoryLayout memory_layout_ = MemoryLayout::RowMajor);

  /** The number of tensor dimensions the data has */
  size_t dimension() const { return shape.size(); }

  /** The shape metadata which was provided on construction */
  std::vector<size_t> shape;

  /** The memory layout, which should be assumed for the data. */
  MemoryLayout memory_layout;
};

}  // namespace pammap
