import unittest

import cubemath


class TestCell(unittest.TestCase):
    def test_cube_distance(self):
        cell1 = (1, 2, -3)
        cell2 = (1, 2, -3)
        actual = cubemath.cube_distance(cell1, cell2)
        expected = 0
        self.assertEqual(expected, actual)

        cell1 = (1, 1, -2)
        cell2 = (0, -3, 3)
        actual = cubemath.cube_distance(cell1, cell2)
        expected = 5
        self.assertEqual(expected, actual)

    def test_offset(self):
        cell = (1, 1, -2)
        shift = (-1, -4, 5)
        actual = cubemath.offset(cell, shift)
        expected = (0, -3, +3)
        self.assertEqual(expected, actual)
        cell = (1, -1, 0)
        shift = (-1, 1, 0)
        actual = cubemath.offset(cell, shift)
        expected = (0, 0, 0)
        self.assertEqual(expected, actual)

    def test_neighbours(self):
        cell = (-3, 0, 3)
        actual = cubemath.get_neighbours(cell)
        self.assertIsInstance(actual, list)
        self.assertEqual(len(actual), len(set(actual)))
        actual_set = set(actual)
        expected_set = {
            (-3, -1, 4),
            (-2, -1, 3),
            (-2, 0, 2),
            (-3, 1, 2),
            (-4, 1, 3),
            (-4, 0, 4),
        }
        self.assertEqual(expected_set, actual_set)

    def test_in_radius(self):
        cell = (-2, 1, 1)
        expected = cubemath.get_neighbours(cell)
        expected.append(cell)
        actual = cubemath.in_radius(cell, 1)
        self.assertEqual(set(expected), set(actual))

        cell = (1, -1, 0)
        expected = []
        for i in cubemath.get_neighbours(cell):
            expected.extend(cubemath.get_neighbours(i))
        actual = cubemath.in_radius(cell, 2)
        self.assertEqual(set(expected), set(actual))

    def test_in_radius_excl(self):
        cell = (2, 1, -3)
        expected = cubemath.get_neighbours(cell)
        actual = cubemath.in_radius_excl(cell, 1, 0)
        self.assertEqual(expected, actual)


if __name__ == "__main__":
    unittest.main()