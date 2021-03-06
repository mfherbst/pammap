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

import os
import subprocess


def list_committed_files():
    """List all files which are commited in the repo located
    in directory"""
    command = "git ls-tree --full-tree --name-only -r HEAD".split(" ")
    output = subprocess.check_output(command, universal_newlines=True)
    return [line for line in output.split("\n") if line != ""]


def get_repo_root_path():
    return subprocess.check_output("git rev-parse --show-toplevel".split(" "),
                                   universal_newlines=True).strip()


def read_ignore_globs(ignore_file):
    """
    Read a file, given relative to the root of the repo
    and return the list of globs of files to be ignored.

    If the file does not exist, returns an empty list.
    """
    if not os.path.isabs(ignore_file):
        ignore_file = os.path.join(get_repo_root_path(), ignore_file)

    if not os.path.isfile(ignore_file):
        return []
    with open(ignore_file) as ignf:
        return [line.strip() for line in ignf if not line.startswith("#")]
