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

cmake_minimum_required(VERSION 3.0.0)
cmake_policy(VERSION 3.0.0)

# Prefer shared to static libs (cmake does the reverse by default)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" ON)

# We need even static library code to be position independent
# as most modules are compiled and integrated with 3rd party
# interpreters as shared modules
if (NOT BUILD_SHARED_LIBS)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

#
##########################################################################
# Project setup

project(pammap VERSION 0.0.0)

# Export compile commands if cmake supports it.
if (CMAKE_VERSION VERSION_GREATER 3.5.0)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

#
##########################################################################
# Find dependencies

include(cmake/SetupCompiler.cmake)
include(cmake/CheckCxxFeatures.cmake)
include(cmake/CheckCxxAbi.cmake)

if (NOT HAVE_CXX_ABI)
	message(FATAL_ERROR "C++ ABI header is required.")
endif()

#
##########################################################################
# Pass on to subdirectories

option(PAMMAP_ENABLE_TESTS "Enable testing of pammap" ON)
if(PAMMAP_ENABLE_TESTS)
	add_subdirectory(external/catch2)
	enable_testing()
endif()

add_subdirectory(pammap)

option(PAMMAP_BUILD_EXAMPLES "Enable building pammap examples" ON)
if(PAMMAP_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
