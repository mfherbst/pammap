#!/usr/bin/env python3
## vi: tabstop=4 shiftwidth=4 softtabstop=4 expandtab
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

import unittest
import swiginterface
import numpy as np


class SwigInterfaceTests(unittest.TestCase):
    def test_num_entries(self):
        m = swiginterface.PamMap()
        self.assertEqual(swiginterface.num_entries(m), 0)
        m.update_integer("first", 1)
        self.assertEqual(swiginterface.num_entries(m), 1)
        m.update_string("sec/ond", "two")
        self.assertEqual(swiginterface.num_entries(m), 2)
        m.update_float("third", 3.)
        self.assertEqual(swiginterface.num_entries(m), 3)

        aint = np.array([1, 2, 3])
        aflt = np.array([4.5, 6.])
        acpx = np.array([4.5, 6. + 5j])

        m.update_integer_array("arr/int", aint)
        self.assertEqual(swiginterface.num_entries(m), 4)
        m.update_float_array("arr/flt", aflt)
        self.assertEqual(swiginterface.num_entries(m), 5)
        m.update_complex_array("arr/cpx", acpx)
        self.assertEqual(swiginterface.num_entries(m), 6)

    def test_max_value(self):
        aint = np.array([1, 2, -3])
        aflt = np.array([4.5, -6., 5.])

        self.assertEqual(swiginterface.max_value_float(aflt), 5.)
        self.assertEqual(swiginterface.max_value_integer(aint), 2)
