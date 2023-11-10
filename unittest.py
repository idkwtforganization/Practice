# -*- coding: utf-8 -*-
"""
Created on Sat Nov 11 00:14:20 2023

@author: Aviator
"""

import unittest
import numpy as np

from NASH import nash_equilibrium

class TestNash(unittest.TestCase):

    def test_1st_game(self):
        game_res = nash_equilibrium(
            np.array(
                [[4, 0, 6, 2, 2, 1],
                [3, 8, 4, 10, 4, 4],
                [1, 2, 6, 5, 0, 0],
                [6, 6, 4, 4, 10, 3],
                [10, 4, 6, 4, 0, 9],
                [10, 7, 0, 7, 9, 8]]
                ))
        self.assertEqual(game_res[0], 151/31)
        np.testing.assert_array_equal(game_res[1], 
                                      np.array([0, 4/31, 3/31, 27/62, 21/62, 0]))
        np.testing.assert_array_equal(game_res[2], 
                                      np.array([0, 0, 257/372, 9/62, 55/372, 1/62]))

    def test_2nd_game(self):
        game_res = nash_equilibrium(
            np.array(
                [[6, 5],
                 [5, 4]]
                ))
        self.assertEqual(game_res[0], 5)
        np.testing.assert_array_equal(game_res[1], np.array([1, 0]))
        np.testing.assert_array_equal(game_res[2], np.array([0, 1]))


if __name__ == "__main__":
  unittest.main()