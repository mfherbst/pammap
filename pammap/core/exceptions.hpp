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
struct AssertionError : public PamMapError {
  AssertionError(pammap_error_constructor_args);
};

/** Error thrown when a dict or a pammap encounters an unknown key */
struct KeyError : public PamMapError {
  KeyError(pammap_error_constructor_args, const std::string& key);
  std::string key;
};

#define declare_description_error(name)                                \
  struct name : public PamMapError {                                   \
    name(pammap_error_constructor_args, std::string description = ""); \
  }

/** Error to flag that a function or a constructor got an invalid value
 *  or an invalid set of arguments and no more specific exception is available.
 */
declare_description_error(ValueError);

/** Error to flag that a function or a constructor had to deal with invalid
 *  or conflicting type information.
 */
declare_description_error(TypeError);

/** Error to flag that in invalid operation is requested, e.g. a release of unowned memory
 *  or the object is in an invalid state and no more specific exception is available */
declare_description_error(InvalidStateError);

/** Error to flag that something is not yet implemented. */
declare_description_error(NotImplementedError);

/** Error to flag that and index is out of range */
declare_description_error(IndexError);

#undef declare_description_error
#undef pammap_error_constructor_args
#undef pammap_error_constructor_vars

//
// Helper macros
//

/** Assert a condition and if it evaluates to false, throw an AssertionError
 */
#define pammap_assert(cond)                                                       \
  {                                                                               \
    if (!(cond)) {                                                                \
      throw AssertionError("AssertionError", __FILE__, __LINE__,                  \
                           static_cast<const char*>(__PRETTY_FUNCTION__), #cond); \
    }                                                                             \
  }

/** Assert a condition and if it evaluates to false, throw the exception
 *  given as the second argument.
 */
#define pammap_throw(cond, exception, ...)                                  \
  {                                                                         \
    if (!(cond)) {                                                          \
      throw exception(#exception, __FILE__, __LINE__,                       \
                      static_cast<const char*>(__PRETTY_FUNCTION__), #cond, \
                      __VA_ARGS__);                                         \
    }                                                                       \
  }

}  // namespace pammap
