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

include(CheckCXXSourceCompiles)
set(ORIGINAL_FLAGS "${CMAKE_REQUIRED_FLAGS}")

#
# --------------------------------------------------------------------
#

set(CMAKE_REQUIRED_FLAGS "-std=c++17")
CHECK_CXX_SOURCE_COMPILES(
	"#include <any>

	int main() {
		int i = 0;
		std::any var = i;
		return std::any_cast<int>(var);
	}"
	HAVE_CXX17_ANY
)

#
# --------------------------------------------------------------------
#

set(CMAKE_REQUIRED_FLAGS "${ORIGINAL_FLAGS}")
unset(ORIGINAL_FLAGS)

#
# Adjust CMAKE_CXX_FLAGS
#
set(TARGET_CXX_FLAG 11)
if (HAVE_CXX17_ANY)
	set(TARGET_CXX_FLAG 17)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${TARGET_CXX_FLAG}")
unset(TARGET_CXX_FLAG)
