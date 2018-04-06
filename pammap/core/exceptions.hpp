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
#include "PamMapError.hpp"

namespace pammap {

#define pammap_error_constructor_args                                     \
  const char *name_, const char *file_, int line_, const char *function_, \
        const char *failed_condition_

#define pammap_error_constructor_vars name_, file_, line_, function_, failed_condition_

/** Error to flag an internal issue, e.g. if an internal assertion fails. */
class AssertionError : public PamMapError {
 public:
  AssertionError(pammap_error_constructor_args);
};

/** Error to flag that a function or a constructor got an invalid value
 *  or an invalid set of arguments and no more specific exception is available.
 */
class ValueError : public PamMapError {
 public:
  ValueError(pammap_error_constructor_args, std::string description = "");
};

/** Error to flag that in invalid operation is requested, e.g. a release of unowned memory
 *  or the object is in an invalid state and no more specific exception is available */
class InvalidStateError : public PamMapError {
 public:
  InvalidStateError(pammap_error_constructor_args, std::string description = "");
};

/** Error thrown when a dict or a pammap encounters an unknown key */
class KeyError : public PamMapError {
 public:
  KeyError(pammap_error_constructor_args, std::string key_);
  std::string key;
};

#undef pammap_error_constructor_args
#undef pammap_error_constructor_vars

/** Assert a condition and if it evaluates to false, throw an AssertionError
 */
#define pammap_assert(cond)                                                           \
  {                                                                                   \
    if (!(cond)) {                                                                    \
      throw AssertionError("AssertionError", __FILE__, __LINE__, __PRETTY_FUNCTION__, \
                           #cond);                                                    \
    }                                                                                 \
  }

/** Assert a condition and if it evaluates to false, throw the exception
 *  given as the second argument.
 */
#define pammap_throw(cond, exception, ...)                                        \
  {                                                                               \
    if (!(cond)) {                                                                \
      throw exception(#exception, __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond, \
                      __VA_ARGS__);                                               \
    }                                                                             \
  }

}  // namespace pammap
