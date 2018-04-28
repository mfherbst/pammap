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

include(CMakeParseArguments)

# TODO Do this setup from a python script to facilitate integration
#      with setuptools and disttools later. This cmake script
#      should only call the appropriate python script in the end.

#
# Setup swig and python
#
find_package(SWIG 2.0.11 REQUIRED)
include(${SWIG_USE_FILE})

# Search for Python interpreter and libraries, since both need to
# be consistent with another
set(Python_ADDITIONAL_VERSIONS 3)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs   REQUIRED)

#
# Functions
#
function(pammap_python_add_module MODULE_NAME)
	# Add a new python module based on a list of header and source files.
	# Additional dependencies can be linked into the module later using
	# pammap_python_link_libraries.
	#
	# HEADERS    The list of header files to be exposed to python.
	# SOURCES    The list of source files containing the corresponding
	#            implementation code.
	#
	set(options)
	set(oneValueArgs)
	set(multiValueArgs HEADERS SOURCES)
	cmake_parse_arguments(PAPM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	# What set of includes to use
	if("${PROJECT_NAME}" STREQUAL "pammap")
		# We are building from within pammap source tree.
		include_directories(
			"${PROJECT_SOURCE_DIR}/pammap/core"
			"${PROJECT_BINARY_DIR}/pammap/core"
			"${PROJECT_SOURCE_DIR}/pammap/python"
			"${PROJECT_BINARY_DIR}/pammap/python"
		)
	else()
		message(FATAL_ERROR "Out of tree pammap_python_add_module not yet supported.")
	endif()

	# TODO Dump Swig interface file to ${MODULE_NAME}.i
	#      use PAPM_HEADERS and PamMap.i

	set_source_files_properties(${MODULE_NAME}.i PROPERTIES CPLUSPLUS ON)
	swig_add_library(${MODULE_NAME} LANGUAGE python SOURCES ${MODULE_NAME}.i ${PAPM_SOURCES})

	SET(SWIG_CXX_FLAGS
		"-Wno-error -Wno-sign-conversion -Wno-conversion \
		-Wno-missing-field-initializers -Wno-pedantic -Wno-old-style-cast \
		-Wno-sign-compare -Wno-maybe-uninitialized -Wno-deprecated -Wno-error \
		-Wno-register -Wno-useless-cast -Wno-duplicated-cond"
	)
	set_source_files_properties(
		${swig_generated_file_fullname}
		PROPERTIES
		COMPILE_FLAGS "${SWIG_CXX_FLAGS}"
	)
	target_include_directories(
		${SWIG_MODULE_${MODULE_NAME}_REAL_NAME}
		PRIVATE ${CMAKE_CURRENT_LIST_DIR};${PAMMAP_PYTHON_INCLUDE_DIRECTORIES}
		PUBLIC ${PYTHON_INCLUDE_PATH}
	)
	swig_link_libraries(${MODULE_NAME} ${PYTHON_LIBRARIES} pammap_core)
endfunction()

function(pammap_python_link_libraries MODULE_NAME)
	swig_link_libraries(${MODULE_NAME} ${ARGN})
endfunction()

function(pammap_python_install_module MODULE_NAME)
	# TODO
endfunction()
