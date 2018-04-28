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

include(../PamMapPython.cmake)

pammap_python_add_module(swiginterface SOURCES swiginterface.cpp HEADERS swiginterface.hpp)

set(PYTHONPATH "${CMAKE_CURRENT_BINARY_DIR}:$ENV{PYTHONPATH}")

add_test(NAME python_interface_tests
	COMMAND ${PYTHON_EXECUTABLE} -m pytest -v "${CMAKE_CURRENT_LIST_DIR}"
)

set_tests_properties(python_interface_tests
	PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHONPATH}"
)
