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

##########################################################################
# Project setup

project(pammap VERSION 0.0.0)
message("\n# Configuring ${PROJECT_NAME}\n#")

##########################################################################
# Setup hard and optional dependencies and find components

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror -Wextra \
-pedantic -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast -Wcast-align \
-Wconversion -Wsign-conversion"
)

# Export compile commands if cmake supports it.
if (CMAKE_VERSION VERSION_GREATER 3.5.0)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# TODO Hard-coded
set(KRIMS_DIR "${PROJECT_SOURCE_DIR}/krims" CACHE STRING "Hard-coded krims directory")
include_directories("${KRIMS_DIR}/src")
include_directories("${KRIMS_DIR}/build/src")
link_directories("${KRIMS_DIR}/build/src/krims")

##########################################################################
# Pass on to subdirectories
add_subdirectory(pammap)

option(PAMMAP_ENABLE_TESTS "Enable tesing of pammap" ON)
if(PAMMAP_ENABLE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

##########################################################################
# Finish off
message("#\n# Configuring ${PROJECT_NAME} done\n")