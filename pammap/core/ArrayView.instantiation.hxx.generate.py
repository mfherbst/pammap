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

from common import licence_header_cpp, NAMESPACE_OPEN, NAMESPACE_CLOSE, to_cpp_type
import constants


def generate():
    output = licence_header_cpp(__file__)
    output += ["#include \"typedefs.hxx\""]

    output += NAMESPACE_OPEN
    for dtype in constants.DTYPES:
        output.append("template struct ArrayView<{0:}>;".format(to_cpp_type(dtype)))
    output += NAMESPACE_CLOSE
    return "\n".join(output)


if __name__ == "__main__":
    genfile = __file__.replace(".generate.py", "")
    with open(genfile, "w") as f:
        f.write(generate())
