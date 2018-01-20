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

# The conceptual data types we support
DTYPES = [
    "complex",
    "integer",
    "float",
    "string"
]


class cpp:
    """Constants for C++"""

    # Headers which are needed for representing the dtypes
    headers = ["<complex>", "<string>", "<cstdint>"]

    # How the conceptual dtypes are represented on C++
    underlying_type = {
        "integer": "int64_t",
        "float": "double",
        "complex": "std::complex<double>",
        "string": "std::string"
    }


class python:
    # How the conceptual dtypes are represented in python
    underlying_type = {
        "integer": "int",
        "double": "float",
        "complex": "complex",
        "string": "str",
    }
