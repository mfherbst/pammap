## ---------------------------------------------------------------------
##
## Copyright (C) 2018 by the pammap authors
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

# This file is loosely based upon the file
# cpp/ycm/.ycm_extra_conf.py from the youcompleteme daemon process
# available on github:
# https://github.com/Valloric/ycmd/blob/master/cpp/ycm/.ycm_extra_conf.py

def FindStdInclude():
  # Find the standard library include directory

  for include in [ "/usr/include/c++" ]:
    for dir in ["v1"] + [ str(ver) for ver in range(4,10,1) ]:
      if os.path.exists(os.path.join(dir,"cstdlib")):
        return dir

  # libc++ include directory:
  return "/usr/include/c++/v1"


def BuildBaseFlags():
  # These are the compilation flags that will be used in case there's no
  # compilation database set (by default, one is not set).
  return [
    # Warnings: For a very detailed discussion about this
    # see the following stackexchange post:
    # https://programmers.stackexchange.com/questions/122608#124574
    '-Wall',
    '-Wextra',
    '-Wnon-virtual-dtor',
    '-Woverloaded-virtual',
    '-Wold-style-cast',
    '-Wcast-align',
    '-Wconversion',
    '-Wsign-conversion',
    '-pedantic',
    '-Werror',
    # Generate unwind information
    '-fexceptions',
    # Compile debug code as well
    '-DDEBUG',
    # Compile as c++17
    '-std=c++17',
    # Treat .h header files as c++:
    '-x', 'c++',
    # Include other libraries and show errors and
    # warnings within them
    # To suppress errors shown here, use "-isystem"
    # instead of "-I"
    '-I', './pammap/core',
    '-isystem', './build',
    '-isystem', FindStdInclude(),
  ]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.C' ]

def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def IsHeaderFile( filename ):
  extension = os.path.splitext( filename )[ 1 ]
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ]

def FlagsForFile( filename, **kwargs ):
  relative_to = DirectoryOfThisScript()
  final_flags = MakeRelativePathsInFlagsAbsolute( BuildBaseFlags(), relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }
