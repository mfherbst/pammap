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

#
# Build
#
set(PAMMAP_SOURCES
	Slice.cpp
	ArrayView.cpp
	PamMap.cpp
	PamMapError.cpp
	PamMapValue.cpp
	demangle.cpp
	exceptions.cpp
)

configure_file("config.hpp.in" "config.hpp")
add_library(pammap_core ${PAMMAP_SOURCES})
set_target_properties(pammap_core PROPERTIES VERSION "${PROJECT_VERSION}")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# install files and target specifications
#
install(TARGETS pammap_core
	EXPORT PammapCoreTargets
	DESTINATION lib
)
install(DIRECTORY .
	DESTINATION "include/pammap"
	FILES_MATCHING
	PATTERN "tests" EXCLUDE
	PATTERN "pammap" EXCLUDE
	PATTERN "__pycache__" EXCLUDE
	PATTERN "*.instantiation.h??" EXCLUDE
	PATTERN "*.hpp" PATTERN "*.hxx"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/config.hpp"
	DESTINATION "include/pammap"
)

export(EXPORT PammapCoreTargets
	FILE "${pammap_BINARY_DIR}/PammapCoreTargets.cmake"
	NAMESPACE Upstream::
)
install(EXPORT PammapCoreTargets
	FILE "PammapCoreTargets.cmake"
	NAMESPACE Upstream::
	DESTINATION "share/cmake/pammap"
)

#
# Testing
#
if (PAMMAP_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
