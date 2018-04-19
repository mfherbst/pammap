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
#include <memory>
#include <vector>

namespace pammap {
enum class Memory {
  /** View the data only, i.e. do not deallocate it later */
  ViewOnly,

  /** Copy the data into internal storage. Delete it when done */
  OwnCopy,
};

/** Build C-like strides from a shape vector */
std::vector<size_t> strides_c(const std::vector<size_t>& shape);

/** Build Fortran-like strides from a shape vector */
std::vector<size_t> strides_fortran(const std::vector<size_t>& shape);

/** Simple low-level class to store a block of data */
template <typename T>
class DataBlock {
 public:
  typedef T value_type;
  typedef T* iterator;
  typedef const T* const_iterator;

  /** Construct an empty object, which is unusable */
  DataBlock()
        : m_data(nullptr),
          m_size(0),
          m_shape(0),
          m_strides(1),
          m_ownership(Memory::OwnCopy) {}

  /** Initialise from iterator range as 1d array by making a copy */
  template <typename Input>
  explicit DataBlock(Input first, Input last)
        : m_data(nullptr),
          m_size(static_cast<size_t>(std::distance(first, last))),
          m_shape{static_cast<size_t>(std::distance(first, last))},
          m_strides{1},
          m_ownership{Memory::OwnCopy} {
    m_data = new T[m_size];
    std::copy(first, last, m_data);
  }

  /** Initialise from initializer_list as 1d array by making a copy */
  DataBlock(std::initializer_list<T> data) : DataBlock(data.begin(), data.end()) {}

  /** Initialise from std::vector as 1d array by making a copy */
  DataBlock(std::vector<T> data) : DataBlock(std::move(data), {data.size()}, {1}) {}

  /** Initialise from vector and shape by making a copy */
  DataBlock(std::vector<T> data, std::vector<size_t> shape)
        : DataBlock(std::move(data), shape, strides_c(shape)) {}

  /** Initialise from vector, shape and strides by making a copy */
  DataBlock(std::vector<T> data, std::vector<size_t> shape, std::vector<size_t> strides);

  /** Initialise by copying from a memory location and a shape */
  DataBlock(T* data_, std::vector<size_t> shape, Memory ownership = Memory::OwnCopy)
        : DataBlock(data_, shape, strides_c(shape), ownership) {}

  /** Initialise by copying from a memory location, a shape and strides */
  DataBlock(T* data_, std::vector<size_t> shape, std::vector<size_t> strides,
            Memory ownership = Memory::OwnCopy);

  /** TODO use copy-and-swap idiom here! */
  DataBlock(DataBlock&&);
  DataBlock& operator=(DataBlock&&);
  DataBlock(const DataBlock& other);
  DataBlock& operator=(const DataBlock&);

  /** Copy the data from another DataBlock.
   *
   * \param ownership  If ViewOnly, this object is only a view
   *                   if OwnCopy, this object will have its own
   *                   copy of the data
   */
  DataBlock(const DataBlock& other, Memory ownership);

  /** Destructor: Delete data if this is the copy holder */
  ~DataBlock() {
    if (m_ownership == Memory::OwnCopy) {
      delete[] m_data;
    }
  }

  /** Return the pointer to the underlying data */
  T* data() { return m_data; }

  /** Return a const pointer to the underlying data */
  const T* data() const { return m_data; }

  //@{
  /** Return an iterator to the beginning of the data */
  iterator begin() { return m_data; }
  const_iterator begin() const { return m_data; }
  const_iterator cbegin() const { return m_data; }
  //@}

  //@{
  /** Return an iterator to the end of the data */
  iterator end() { return m_data + m_size; }
  const_iterator end() const { return m_data + m_size; }
  const_iterator cend() const { return m_data + m_size; }
  //@}

  /** Dimensionality */
  size_t ndim() const { return m_shape.size(); }

  /** Total size of the data */
  size_t size() const { return m_size; }

  /** Strides of the data */
  std::vector<size_t> strides() const { return m_strides; }

  /** Shape of the data */
  std::vector<size_t> shape() const { return m_shape; }

  /** Is the data Fortran contiguous */
  bool is_fortran_contiguous() const;

  /** Is the data c contiguous */
  bool is_c_contiguous() const;

 private:
  /** The pointer to the actual data */
  T* m_data;

  /** The size of the data */
  size_t m_size;

  /** The shape metadata which was provided on construction */
  std::vector<size_t> m_shape;

  /** The stride metadata which was computed on construction */
  std::vector<size_t> m_strides;

  /** The memory ownership status */
  Memory m_ownership;
};

}  // namespace pammap
