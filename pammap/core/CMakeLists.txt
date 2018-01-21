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

##############
#-- pammap --#
##############

# C++ sourle files in the repository
set(PAMMAP_SOURCES
	DataBlock.cpp
	PamMapValue.cpp
	demangle.cpp
	PamMap.cpp
)

# Build the C++ library
include_directories("${PROJECT_SOURCE_DIR}")
add_library(pammap ${PAMMAP_SOURCES})
set_target_properties(pammap PROPERTIES VERSION "${PROJECT_VERSION}")

# TODO Dependency for now.
target_link_libraries(pammap PUBLIC krims)

############
#-- SWIG --#
############
# Check for SWIG, at least 2.0.11
find_package(SWIG 2.0.11 REQUIRED)
include(${SWIG_USE_FILE})

# Search for both interpreter and libraries, since both need to
# be consistent with another
set(Python_ADDITIONAL_VERSIONS 3)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs   REQUIRED)

# Download the numpy.i module we require from github
file(DOWNLOAD https://raw.githubusercontent.com/numpy/numpy/v1.13.1/tools/swig/numpy.i
	${CMAKE_CURRENT_BINARY_DIR}/numpy.i
	EXPECTED_HASH
	SHA256=bc658c3e9f4ecc5343041339c0697314398fd7cd0e442aa2d424b71086e305ec
)

# Setup swig library/module
set(MODULE_NAME pammap_core)
set_source_files_properties(${MODULE_NAME}.i PROPERTIES CPLUSPLUS ON)

if (CMAKE_VERSION VERSION_GREATER 3.8)
	swig_add_library(${MODULE_NAME} LANGUAGE python
		SOURCES ${MODULE_NAME}.i)
else()
	# Older version of the call, deprecated from cmake 3.9
	swig_add_module(${MODULE_NAME} python)
endif()


# Compiler flags for SWIG
set_source_files_properties(
	${swig_generated_file_fullname}
	PROPERTIES
	COMPILE_FLAGS
	"-Wno-sign-conversion -Wno-conversion -Wno-missing-field-initializers -Wno-pedantic -Wno-old-style-cast -Wno-sign-compare -Wno-maybe-uninitialized -Wno-deprecated -Wno-error"
)
target_include_directories(
	${SWIG_MODULE_${MODULE_NAME}_REAL_NAME}
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}
	PUBLIC ${PYTHON_INCLUDE_PATH}
)

swig_link_libraries(${MODULE_NAME} ${PYTHON_LIBRARIES} pammap)
