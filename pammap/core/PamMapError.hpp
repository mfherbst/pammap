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
#include <exception>
#include <string>

namespace pammap {

class PamMapError : public std::exception {
 public:
  /** Constructor */
  PamMapError(const char* name_, const char* file_, int line_, const char* function_,
              const char* failed_condition_, std::string extra);

  /** Default copy constructor */
  PamMapError(const PamMapError&) = default;

  /** Default destructor */
  virtual ~PamMapError() noexcept = default;

  /** The c-string which describes briefly what happened.
   *
   * This string includes information about what assertion has failed,
   * the name of the exception, where the exception has failed and
   * the exception-specific extra information */
  const char* what() const noexcept { return what_str.c_str(); }

  //! The name of the exception
  const char* name;

  //! The file where exception occurred
  const char* file;

  //! The line where exception occurred
  int line;

  //! The name of the function
  const char* function;

  //! The failed condition as a string.
  const char* failed_condition;

  //! The extra information, specific to the exception.
  std::string extra;

  //! The what of the exception as a std::string
  std::string what_str;
};

}  // namespace pammap
