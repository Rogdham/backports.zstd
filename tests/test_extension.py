import os
import unittest

from backports import zstd


@unittest.skipIf(
    os.environ.get("BACKPORTSZSTD_SKIP_EXTENSION_TEST") == "1",
    "BACKPORTSZSTD_SKIP_EXTENSION_TEST set",
)
class TestExtension(unittest.TestCase):
    def test_multithreading_support(self):
        self.assertFalse(zstd.CompressionParameter.nb_workers.bounds() == (0, 0))

    def test_zstd_version(self):
        self.assertEqual(zstd.zstd_version, "1.6.0")
        self.assertEqual(zstd.zstd_version_info, (1, 6, 0))


if __name__ == "__main__":
    unittest.main()
