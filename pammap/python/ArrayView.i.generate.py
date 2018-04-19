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

from common import to_cpp_blocktype
import constants
import os

thisdir = os.path.dirname(__file__)


def generate():
    with open(os.path.join(thisdir, "ArrayView.i.template")) as f:
        original = f.readlines()

    output = [""]
    for dtype in constants.DTYPES:
        if dtype not in constants.python.underlying_numpy_type:
            continue
        nptype = constants.python.underlying_numpy_type[dtype]
        output += [
            "%arrayview_typemaps(" + to_cpp_blocktype(dtype, full=True) +
            ", " + nptype + ")"
        ]
    return "".join(original) + "\n".join(output)


if __name__ == "__main__":
    genfile = __file__.replace(".generate.py", "")
    with open(genfile, "w") as f:
        f.write(generate())
