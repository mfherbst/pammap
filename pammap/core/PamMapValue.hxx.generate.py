#!/usr/bin/env python3
## vi: tabstop=4 shiftwidth=4 softtabstop=4 expandtab
## ---------------------------------------------------------------------
##
## Copyright (C) 2018 by Michael F. Herbst and contributors
##
## This file is part of pammap.
##
## pammap is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published
## by the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## pammap is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with pammap. If not, see <http://www.gnu.org/licenses/>.
##
## ---------------------------------------------------------------------

from common import licence_header_cpp, NAMESPACE_OPEN, NAMESPACE_CLOSE, clean_block
from common import make_supported_cpp_types
import constants


def generate():
    output = licence_header_cpp(__file__)
    output += [
        r"#pragma once",
        r'#include "ArrayView.hpp"',
        r'#include "IsSupportedType.hxx"',
        r'#include "any.hpp"',
        r'#include "typedefs.hxx"',
    ]
    output += NAMESPACE_OPEN

    # Add class header
    output += clean_block(r"""
    /** \brief Class to contain an entry value in a PamMap.
        Essentially a slightly specialised pammap::any */
    class PamMapValue : public any {
     public:
    """)

    # Add fallback constructors
    output += clean_block(r"""
      PamMapValue() = default;

      /** Catch-all constructor, which defaults to an error */
      template <typename ValueType>
      PamMapValue(ValueType) : PamMapValue() {
        static_assert(!std::is_unsigned<ValueType>::value,
                      "Unsigned integer types are not supported with PamMap. "
                      "Use a signed type instead.");

        static_assert(IsSupportedType<ValueType>::value,
                      "This value type is not supported by PamMap.");
      }
    """)
    output.append("")

    # Auto-generate constructors
    for cpptype in make_supported_cpp_types(constants.DTYPES):
        output += [
            "  /** Construction from " + cpptype + " */",
            "  PamMapValue(" + cpptype + " val) : any(std::move(val)) {}",
            ""
        ]

    # Add transforming constructors
    output += clean_block(r"""
      //
      // The int type gets special treatment because it is the default for raw numbers
      //
      /** \brief Make an PamMapValue out of an int. Behaves like a PamMapValue
        *        containing an INTEGER type */
      PamMapValue(int i) : PamMapValue(static_cast<Integer>(i)) {}

      //
      // Same for const char*
      //
      /** \brief Make an PamMapValue out of a const char*.
        * This behaves like the equivalent GenMapValue of a  std::string */
      PamMapValue(const char* s) : PamMapValue(std::string(s)) {}
    """)
    output.append("")

    # Add type_name() method
    output += clean_block(r"""
      /** Return the demangled typename of the type of the internal object. */
      std::string type_name() const;
    """)

    # Close class and namespace
    output += ["};"]
    output += NAMESPACE_CLOSE
    return "\n".join(output)


if __name__ == "__main__":
    genfile = __file__.replace(".generate.py", "")
    with open(genfile, "w") as f:
        f.write(generate())
