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

import argparse
import common
import os
import subprocess


def main():
    parser = argparse.ArgumentParser(
        description="Run clang-format on all currently committed c++ files"
    )
    parser.add_argument("-clang-format", metavar="PATH", default="clang-format",
                        help="Name or path of clang-format executable to use.")
    args = parser.parse_args()

    cpp_extensions = [".cpp", ".hpp", ".cxx", ".hxx", ".hh", ".cc", ".h", ".c"]
    root = common.get_repo_root_path()
    cpp_files = [
        os.path.join(root, file) for file in common.list_committed_files()
        if os.path.splitext(file)[1] in cpp_extensions
    ]

    commandline = [args.clang_format, "-style=file", "-i"]
    for cfile in cpp_files:
        subprocess.check_call(commandline + [cfile])


if __name__ == "__main__":
    main()
