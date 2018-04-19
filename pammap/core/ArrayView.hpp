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
#include <type_traits>
#include <vector>

namespace pammap {

// TODO One could at some point go as far as integrating with the reference counting of
//      numpy arrays is well.
//      By the means of the register function a base can be registered and the counter
//      increased and conversely with the unregister function.

class ArrayViewBase {
 public:
  /** Enum to mark the kind of object the base pointer points to */
  enum BASE_KIND {
    NONE  = 0,
    NUMPY = 1,
  };

  ArrayViewBase() = default;
  ArrayViewBase(std::vector<size_t> shape_, std::vector<size_t> strides_);

  /** The number of elements */
  size_t size() const { return m_size; }

  /** The number of dimensions */
  size_t ndim() const { return m_shape.size(); }

  /** The shape in each dimension */
  const std::vector<size_t>& shape() const { return m_shape; }

  /** The strides in each dimension */
  const std::vector<size_t>& strides() const { return m_strides; }

  /** Reset the base object, i.e. replace it by a new one */
  void reset_base(void* base, BASE_KIND base_kind);

  /** Reset the base object, i.e. purge the current reference */
  void reset_base();

  /** Obtain the pointer to the current base object */
  void* base() const { return m_base; }

  /** Type of the base object */
  BASE_KIND base_kind() const { return m_base_kind; }

  /** Is the striding Fortran contiguous */
  bool is_fortran_contiguous() const;

  /** Is the striding C contiguous */
  bool is_c_contiguous() const;

 protected:
  /** The size, i.e. total number of elements */
  size_t m_size = 0;

  /** The shape in each dimension */
  std::vector<size_t> m_shape;

  /** The strides in each dimension */
  std::vector<size_t> m_strides;

  /** Some extra pointer to additional structure,
   *  e.g. the original numpy PyArrayObject, to which the data belongs */
  void* m_base = nullptr;

  /** What kind of object does the base pointer point to? */
  BASE_KIND m_base_kind = NONE;
};

/** Light-weight class to view some array data. */
template <typename T>
struct ArrayView : public ArrayViewBase {
  /** The type of the ArrayView data */
  typedef T value_type;

  /** The pointer to the data */
  T* data = nullptr;

  ArrayView() = default;
  ArrayView(T* data_, std::vector<size_t> shape_, std::vector<size_t> strides_)
        : ArrayViewBase{shape_, strides_}, data(data_) {}

  /** Constructor from a std::vector, which is only active for a non-bool std::vector
   *
   * The reason for explicitly excluding std::vector<bool> is that the memory layout is
   * different.
   *
   * TODO Think if there is a nicer way to achieve this.
   */
  template <typename U = T>
  ArrayView(std::vector<typename std::enable_if<!std::is_same<U, bool>::value, T>::type>&
                  vector)
        : ArrayViewBase{{vector.size()}, {1}}, data{vector.data()} {}

  /** Compare two ArrayView objects for equality.
   *
   * They are considered equal if their data and shape/strides are equal
   */
  bool operator==(const ArrayView& other) const;

  /** Compare two ArrayView objects for unequality.
   *
   * Two ArrayViews are considered equal if their data and shape/strides are equal
   */
  bool operator!=(const ArrayView& other) const { return !operator==(other); }
};

}  // namespace pammap
