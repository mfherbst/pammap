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
#include "config.hpp"

#ifdef HAVE_CXX17_ANY
#include <any>

namespace pammap {
using std::any;
using std::any_cast;
using std::bad_any_cast;
}  // namespace pammap

#else

//
// This part of the implementation is taken from libint2 by
// Ed Valeev with minor modifications. The original file can be found at
// https://github.com/evaleev/libint/blob/v2.4.2/include/libint2/util/any.h
// and was released under the terms of the LGPL 3.
//
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace pammap {

namespace detail {
/** True if decayed T is Base, or is derived from it */
template <typename Base, typename T>
using disable_if_same_or_derived = typename std::enable_if<
      !std::is_base_of<Base, typename std::decay<T>::type>::value>::type;
}  // namespace detail

/** A partial C++17 std::any implementation (and less efficient than can be) */
class any {
 public:
  // this is constexpr in the standard
  any() : m_impl(nullptr) {}
  any(const any& other) : m_impl(other.m_impl->clone()) {}
  any(any&& other) = default;
  template <typename ValueType,
            typename = detail::disable_if_same_or_derived<any, ValueType>>
  any(ValueType&& value)
        : m_impl(new impl<typename std::decay<ValueType>::type>(
                std::forward<ValueType>(value))) {}
  ~any() = default;

  any& operator=(const any& rhs) {
    m_impl = decltype(m_impl)(rhs.m_impl->clone());
    return *this;
  }
  any& operator=(any&& rhs) {
    m_impl = std::move(rhs.m_impl);
    return *this;
  }
  template <typename ValueType,
            typename = detail::disable_if_same_or_derived<any, ValueType>>
  any& operator=(ValueType&& rhs) {
    m_impl = decltype(m_impl)(
          new impl<typename std::decay<ValueType>::type>(std::forward<ValueType>(rhs)));
    return *this;
  }

  template <class ValueType, class... Args>
  typename std::decay<ValueType>::type& emplace(Args&&... args) {
    reset();
    m_impl = new impl<typename std::decay<ValueType>::type>(std::forward<Args>(args)...);
    return (m_impl->cast_static<typename std::decay<ValueType>::type>()->value);
  }
  template <class ValueType, class U, class... Args>
  typename std::decay<ValueType>::type& emplace(std::initializer_list<U> il,
                                                Args&&... args) {
    reset();
    m_impl =
          new impl<typename std::decay<ValueType>::type>(il, std::forward<Args>(args)...);
    return (m_impl->cast_static<typename std::decay<ValueType>::type>()->value);
  }

  void reset() { m_impl.reset(); }

  void swap(any& other) { std::swap(m_impl, other.m_impl); }

  bool has_value() const { return static_cast<bool>(m_impl); }

  const std::type_info& type() const {
    if (has_value()) {
      return m_impl->type();
    }
    return typeid(void);
  }

 private:
  template <typename T>
  struct impl;

  struct impl_base {
    virtual ~impl_base() {}
    virtual impl_base* clone() const = 0;

    virtual const std::type_info& type() const = 0;

    // static if NDEBUG is defined, dynamic otherwise
    template <typename T>
    impl<T>* cast() {
#ifndef NDEBUG
      return this->cast_static<T>();
#else
      return dynamic_cast<impl<T>*>(this);
#endif
    }
    // static always
    template <typename T>
    impl<T>* cast_static() {
      return static_cast<impl<T>*>(this);
    }
  };
  template <typename T>
  struct impl : public impl_base {
    template <typename U>
    explicit impl(U&& v) : value(std::forward<U>(v)) {}
    impl_base* clone() const override { return new impl{value}; }

    const std::type_info& type() const override { return typeid(T); }

    T value;
  };

  template <typename ValueType>
  friend typename std::decay<ValueType>::type* any_cast(any* operand);
  template <typename ValueType>
  friend const typename std::decay<ValueType>::type* any_cast(const any* operand);

  template <typename ValueType>
  typename std::decay<ValueType>::type* value_ptr() {
    return &(m_impl->cast_static<typename std::decay<ValueType>::type>()->value);
  }

  template <typename ValueType>
  const typename std::decay<ValueType>::type* value_ptr() const {
    return &(m_impl->cast_static<typename std::decay<ValueType>::type>()->value);
  }

  std::unique_ptr<impl_base> m_impl;
};

class bad_any_cast : public std::bad_cast {
 public:
  bad_any_cast() = default;
  virtual ~bad_any_cast() {}
  virtual const char* what() const noexcept { return "Bad any_cast"; }
};

template <typename ValueType>
typename std::decay<ValueType>::type* any_cast(any* operand) {
  if (operand->type() == typeid(typename std::decay<ValueType>::type)) {
    return operand->value_ptr<typename std::decay<ValueType>::type>();
  }
  return nullptr;
}

template <typename ValueType>
const typename std::decay<ValueType>::type* any_cast(const any* operand) {
  if (operand->type() == typeid(typename std::decay<ValueType>::type)) {
    return operand->value_ptr<typename std::decay<ValueType>::type>();
  }
  return nullptr;
}

template <typename ValueType>
ValueType any_cast(const any& operand) {
  const auto* cast_ptr = any_cast<typename std::decay<ValueType>::type>(&operand);
  if (cast_ptr != nullptr) return *cast_ptr;
  throw bad_any_cast();
}

template <typename ValueType>
ValueType any_cast(any& operand) {
  auto* cast_ptr = any_cast<typename std::decay<ValueType>::type>(&operand);
  if (cast_ptr != nullptr) return *cast_ptr;
  throw bad_any_cast();
}

}  // namespace pammap
#endif  // HAVE_CXX17_ANY
