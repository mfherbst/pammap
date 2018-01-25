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

# TODO Part of this should be a cmake module

# Check for SWIG, at least 2.0.11
find_package(SWIG 2.0.11 REQUIRED)
include(${SWIG_USE_FILE})

# Search for both interpreter and libraries, since both need to
# be consistent with another
set(Python_ADDITIONAL_VERSIONS 3)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs   REQUIRED)

# C++ and swig includes
include_directories("${PROJECT_SOURCE_DIR}/pammap/core")
include_directories("${PROJECT_BINARY_DIR}/pammap/core")

##################
#-- Testmodule --#
##################
set(MODULE_NAME testmodule)
set_source_files_properties(${MODULE_NAME}.i PROPERTIES CPLUSPLUS ON)
swig_add_library(${MODULE_NAME} LANGUAGE python SOURCES testmodule.i testmodule.cpp)
set_source_files_properties(
	${swig_generated_file_fullname}
	PROPERTIES
	COMPILE_FLAGS
	"-Wno-sign-conversion -Wno-conversion -Wno-missing-field-initializers -Wno-pedantic -Wno-old-style-cast -Wno-sign-compare -Wno-maybe-uninitialized -Wno-deprecated -Wno-error"
	# TODO Do not hardcode
)
target_include_directories(
	${SWIG_MODULE_${MODULE_NAME}_REAL_NAME}
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}
	PUBLIC ${PYTHON_INCLUDE_PATH}
)
swig_link_libraries(${MODULE_NAME} ${PYTHON_LIBRARIES} pammap)
