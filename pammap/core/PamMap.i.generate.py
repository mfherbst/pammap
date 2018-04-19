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

from common import licence_header_cpp, to_cpp_type, to_cpp_blocktype
import constants


def generate():
    output = ["// vi: syntax=c"]
    output += licence_header_cpp(__file__)
    output += [
        "",
        "%{",
        '#include "PamMap.hpp"',
        "%}",
        "",
        '%include "pammap_exceptions.i"',
        '%include "ArrayView.i"',
        '%include "std_string.i"',
        '%include "stdint.i"',
        '%include "typedefs.hxx"',
    ]

    output += [
        "",
    ]
    for dtype in constants.DTYPES:
        if dtype not in constants.python.underlying_numpy_type:
            continue
        dbtype = to_cpp_blocktype(dtype, full=True)
        output += ["%apply (" + dbtype + " ARRAYVIEW) {(" + dbtype + " view)}"]

    output += [
        "",
        '%include "PamMap.hpp"',
        "%extend pammap::PamMap {"
    ]
    for dtype in constants.DTYPES:
        cpptype = to_cpp_type(dtype, full=True)
        output += [
            "  void update_" + dtype + "(std::string key, " + cpptype + " value) {",
            "    $self->update(key, std::move(value));",
            "  }",
            "  " + cpptype + " get_" + dtype + "(std::string key) {",
            "    return $self->at<" + cpptype + ">(key);",
            "  }",
            ""
        ]

    for dtype in constants.DTYPES:
        if dtype not in constants.python.underlying_numpy_type:
            continue
        dbtype = to_cpp_blocktype(dtype, full=True)
        # Function to update the value in the PamMap by viewing the data
        output += [
            "void update_" + dtype + "_array(std::string key, " +
            dbtype + " view) {",
            "  $self->update(key, std::move(view));",
            "}",
            ""
        ]

        # Function to retrieve the value in the PamMap (as a copy)
        output += [
            dbtype + " get_" + dtype + "_array(std::string key) {",
            "  return $self->at<" + dbtype + ">(key);"
            "}",
            ""
        ]

    output += ["}"]
    return "\n".join(output)


if __name__ == "__main__":
    genfile = __file__.replace(".generate.py", "")
    with open(genfile, "w") as f:
        f.write(generate())
