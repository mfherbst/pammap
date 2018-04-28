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

# Standard flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror \
-Wall -Wextra -Wnon-virtual-dtor -pedantic \
-Woverloaded-virtual -Wold-style-cast -Wcast-align \
-Wconversion -Wsign-conversion -Wno-unused-variable -Wmisleading-indentation \
-Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference \
-Wuseless-cast -Wdouble-promotion -Wformat=2 \
-Wno-unknown-warning-option \
")

