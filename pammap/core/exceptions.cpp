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

#include "exceptions.hpp"

#define pammap_error_constructor_args                                     \
  const char *name_, const char *file_, int line_, const char *function_, \
        const char *failed_condition_

#define pammap_error_constructor_vars name_, file_, line_, function_, failed_condition_

namespace pammap {

AssertionError::AssertionError(pammap_error_constructor_args)
      : PamMapError(pammap_error_constructor_vars,
                    "An internal consistency assertion inside a routine has failed. "
                    "This is a bug and should not have happened. Please report it to the "
                    "developers along with details how you got this message.") {}

KeyError::KeyError(pammap_error_constructor_args, std::string key_)
      : PamMapError(pammap_error_constructor_vars, "Unknown key: " + key_), key(key_) {}

#define define_description_error(name)                               \
  name::name(pammap_error_constructor_args, std::string description) \
        : PamMapError(pammap_error_constructor_vars, std::move(description)) {}

/* clang-format off */
define_description_error(InvalidStateError)
define_description_error(NotImplementedError)
define_description_error(TypeError)
define_description_error(ValueError)
/* clang-format on */

}  // namespace pammap
