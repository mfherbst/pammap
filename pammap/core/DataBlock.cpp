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
#include "exceptions.hpp"

namespace pammap {

// TODO This code needs proper refactoring

std::vector<size_t> strides_c(const std::vector<size_t>& shape) {
  std::vector<size_t> strides(shape.size(), 0);
  size_t acc = 1;
  for (size_t i = 0; i < shape.size(); ++i) {
    strides[i] = acc;
    acc *= shape[i];
  }
  return strides;
}

std::vector<size_t> strides_fortran(const std::vector<size_t>& shape) {
  const size_t size = shape.size();
  std::vector<size_t> strides(size, 0);
  size_t acc = 1;
  for (size_t i = 1; i <= size; ++i) {
    strides[size - i] = acc;
    acc *= shape[size - i];
  }
  return strides;
}

template <typename T>
DataBlock<T>::DataBlock(std::vector<T> data, std::vector<size_t> shape,
                        std::vector<size_t> strides)
      : m_data(nullptr),
        m_size(data.size()),
        m_shape(std::move(shape)),
        m_strides(std::move(strides)),
        m_ownership{Memory::OwnCopy} {
  size_t acc = 1;
  for (size_t i = 0; i < m_shape.size(); ++i) {
    acc *= m_shape[i];
  }
  pammap_throw(m_size != acc, ValueError,
               "Size of the data (== " + std::to_string(m_size) +
                     ") does not agree with the total number of entries computed by the "
                     "shape (== " +
                     std::to_string(acc) + ").");

  // TODO Check strides

  m_data = new T[m_size];
  std::copy(data.begin(), data.end(), m_data);
}

template <typename T>
DataBlock<T>::DataBlock(T* data, std::vector<size_t> shape, std::vector<size_t> strides,
                        Memory ownership)
      : m_data(nullptr),
        m_size(0),
        m_shape(std::move(shape)),
        m_strides(std::move(strides)),
        m_ownership{ownership} {
  size_t m_size = 1;
  for (size_t i = 0; i < shape.size(); ++i) {
    m_size *= m_shape[i];
  }

  if (m_ownership == Memory::ViewOnly) {
    m_data = data;
  } else {
    m_data = new T[m_size];
    std::copy(data, data + m_size, m_data);
  }
}

template <typename T>
DataBlock<T>::DataBlock(DataBlock&& other)
      : m_data(other.m_data),
        m_size(other.m_size),
        m_shape(other.m_shape),
        m_strides(other.m_strides),
        m_ownership(other.m_ownership) {
  other.m_data = nullptr;
}

template <typename T>
DataBlock<T>& DataBlock<T>::operator=(DataBlock&& other) {
  m_size       = other.m_size;
  m_shape      = other.m_shape;
  m_strides    = other.m_strides;
  m_ownership  = other.m_ownership;
  m_data       = other.m_data;
  other.m_data = nullptr;

  return *this;
}

template <typename T>
DataBlock<T>::DataBlock(const DataBlock& other)
      : m_data(nullptr),
        m_size(other.m_size),
        m_shape(other.m_shape),
        m_strides(other.m_strides),
        m_ownership(other.m_ownership) {
  if (m_ownership == Memory::ViewOnly) {
    m_data = other.m_data;
  } else {
    m_data = new T[m_size];
    std::copy(other.begin(), other.end(), m_data);
  }
}

template <typename T>
DataBlock<T>& DataBlock<T>::operator=(const DataBlock& other) {
  if (m_ownership != Memory::ViewOnly) {
    delete[] m_data;
  }

  m_size      = other.m_size;
  m_shape     = other.m_shape;
  m_strides   = other.m_strides;
  m_ownership = other.m_ownership;

  if (m_ownership == Memory::ViewOnly) {
    m_data = other.m_data;
  } else {
    m_data = new T[m_size];
    std::copy(other.begin(), other.end(), m_data);
  }
  return *this;
}

template <typename T>
DataBlock<T>::DataBlock(const DataBlock& other, Memory ownership)
      : m_data(nullptr),
        m_size(other.m_size),
        m_shape(other.m_shape),
        m_strides(other.m_strides),
        m_ownership(ownership) {
  if (m_ownership == Memory::ViewOnly) {
    m_data = other.m_data;
  } else {
    m_data = new T[m_size];
    std::copy(other.begin(), other.end(), m_data);
  }
}

template <typename T>
bool DataBlock<T>::is_fortran_contiguous() const {
  if (ndim() == 1) return true;
  const size_t size = m_shape.size();

  size_t acc = 1;
  for (size_t i = 1; i <= size; ++i) {
    if (m_strides[size - i] != acc) return false;
    acc *= m_shape[size - i];
  }
  return true;
}

template <typename T>
bool DataBlock<T>::is_c_contiguous() const {
  if (ndim() == 1) return true;

  size_t acc = 1;
  for (size_t i = 0; i < m_shape.size(); ++i) {
    if (m_strides[i] != acc) return false;
    acc *= m_shape[i];
  }
  return true;
}

}  // namespace pammap

#include "DataBlock.instantiation.hxx"
