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

# Module to setup catch test targets
include(${CATCH_DIR}/ParseAndAddCatchTests.cmake)

# To find the headers of the core library
include_directories(..)

add_executable(test_pammap_core
	test.cpp
	SliceTests.cpp
	ArrayViewTests.cpp
	PamMapTests.cpp
	main.cpp
)
target_link_libraries(test_pammap_core pammap_core Catch)
ParseAndAddCatchTests(test_pammap_core)
