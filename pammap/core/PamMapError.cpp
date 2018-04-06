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

#include "PamMapError.hpp"
#include <sstream>

namespace pammap {

PamMapError::PamMapError(const char* name_, const char* file_, int line_,
                         const char* function_, const char* failed_condition_,
                         std::string extra)
      : name(name_),
        file(file_),
        line(line_),
        function(function_),
        failed_condition(failed_condition_),
        extra(std::move(extra)),
        what_str{""} {
  try {
    std::ostringstream converter;

    // Print basic exception data:
    converter << "The assertion" << '\n'
              << "   " << failed_condition << '\n'
              << "failed in line " << line << R"( of file ")" << file
              << R"(" while executing the function)" << '\n'
              << "   " << function << '\n'
              << "This raised the exception" << '\n'
              << "   " << name << '\n'
              << '\n'
              << "Extra information:" << '\n'
              << extra << '\n';

    // Set the what string:
    what_str = converter.str();
  } catch (...) {
    // Default string with some extra nulls at the end in case of
    // accidentially overwritten memory at the end of the string
    what_str = "Failed to generate the exception message.\n\0\0\0";  // NOLINT
  }
}

}  // namespace pammap
