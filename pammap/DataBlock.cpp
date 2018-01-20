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
DataBlock<T>::DataBlock(std::vector<T> data, std::vector<size_t> shape)
      : m_data(nullptr),
        m_size(data.size()),
        m_shape(shape),
        m_strides(shape.size(), 0),
        m_ownership{Memory::OwnCopy} {
  size_t acc = 1;
  for (size_t i = 0; i < shape.size(); ++i) {
    m_strides[i] = acc;
    acc *= shape[i];
  }
  assert_size(acc, m_size);

  m_data = new T[m_size];
  std::copy(data.begin(), data.end(), m_data);
}

template <typename T>
DataBlock<T>::DataBlock(T* data, std::vector<size_t> shape, Memory ownership)
      : m_data(nullptr),
        m_size(0),
        m_shape(shape),
        m_strides(shape.size(), 0),
        m_ownership{ownership} {
  size_t acc = 1;
  for (size_t i = 0; i < shape.size(); ++i) {
    m_strides[i] = acc;
    acc *= shape[i];
  }
  m_size = acc;

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
std::unique_ptr<T[]> DataBlock<T>::release() {
  assert_throw(m_ownership == Memory::OwnCopy,
               krims::ExcInvalidState("Can only release memory if I am the owner."));
  return std::unique_ptr<T[]>(m_data);
  m_data      = nullptr;
  m_ownership = Memory::ViewOnly;
}

}  // namespace pammap

#include "DataBlock.instantiation.hxx"
