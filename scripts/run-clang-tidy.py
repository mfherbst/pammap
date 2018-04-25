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
import sys


def read_ignored_files():
    ignore_file = os.path.join(common.get_repo_root_path(), ".clang-tidy.ignore")
    if not os.path.isfile(ignore_file):
        return []
    with open(ignore_file) as ignf:
        return [line.strip() for line in ignf
                if not line.startswith("#")]


def run_check_output(commandline):
    return_code = 0
    run_clang_tidy = subprocess.Popen(commandline, stdout=subprocess.PIPE,
                                      stderr=subprocess.STDOUT, universal_newlines=True)
    for line in run_clang_tidy.stdout:
        if "warning:" in line or "error:" in line:
            return_code = 1
        sys.stdout.write(line)

    if run_clang_tidy.returncode and run_clang_tidy.returncode != 0:
        raise SystemExit(run_clang_tidy.returncode)
    raise SystemExit(return_code)


def main():
    parser = argparse.ArgumentParser(
        description="Run clang-tidy on all currently committed c++ files"
    )
    parser.add_argument("-p", metavar="BUILD_PATH",
                        default=os.path.join(common.get_repo_root_path(), "build"),
                        help="Path to the build directory")
    parser.add_argument("-j", metavar="THREADS",
                        help="Number of threads to run in parallel")
    parser.add_argument("-export-fixes", metavar="PATH",
                        help="Path to the file where fixes should be stored")
    parser.add_argument("-run-clang-tidy", metavar="PATH", default="run-clang-tidy",
                        help="Name or path of run-clang-tidy executable to use.")
    parser.add_argument("-clang-tidy", metavar="PATH", default="clang-tidy",
                        help="Name or path of clang-tidy executable to use.")
    parser.add_argument("-fix", action="store_true",
                        help="Attempt to fix the issues")
    parser.add_argument("-check-output", action="store_true",
                        help="Check output for warnings and errors and exit with "
                        "non-zero return code if such are found. Disables "
                        "formatted output.")
    args = parser.parse_args()

    cpp_extensions = [".cpp", ".hpp", ".cxx", ".hxx", ".hh", ".cc", ".h", ".c"]
    ignored_files = read_ignored_files()
    cpp_files = [file for file in common.list_committed_files()
                 if os.path.splitext(file)[1] in cpp_extensions and
                 file not in ignored_files
                 ]

    commandline = [args.run_clang_tidy, "-p", args.p, "-clang-tidy", args.clang_tidy]
    if args.j:
        commandline.extend(["-j", args.j])
    if args.export_fixes:
        commandline.extend(["-export-fixes", args.export_fixes])
    if args.fix:
        commandline.extend(["-fix", "-format"])
    commandline.extend(cpp_files)

    if args.check_output:
        run_check_output(commandline)
    else:
        subprocess.check_call(commandline)


if __name__ == "__main__":
    main()
