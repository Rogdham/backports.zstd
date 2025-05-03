# FIXME: remove this file

import unittest

from backports.zstd import ZstdCompressor


class TestFixme(unittest.TestCase):
    def test_fixme(self) -> None:
        self.assertEqual(
            ZstdCompressor().compress(b"foo", ZstdCompressor.FLUSH_FRAME),
            b"(\xb5/\xfd \x03\x19\x00\x00foo",
        )


if __name__ == "__main__":
    unittest.main()
