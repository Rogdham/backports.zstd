import unittest
from backports import zstd


class TestExtension(unittest.TestCase):
    def test_multithreading_support(self):
        self.assertFalse(zstd.CompressionParameter.nb_workers.bounds() == (0, 0))

    def test_zstd_version(self):
        self.assertEqual(zstd.zstd_version, "1.5.7")
        self.assertEqual(zstd.zstd_version_info, (1, 5, 7))


if __name__ == "__main__":
    unittest.main()
