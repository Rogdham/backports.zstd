import sys
import unittest
from io import BytesIO
from secrets import token_bytes, token_urlsafe

if sys.version_info < (3, 14):
    from backports import zstd
else:
    from compression import zstd


# these tests are simple checks for main use cases
# to make sure they work with the conditional import in 3.14 as well


class TestCompat(unittest.TestCase):
    def test_compress_decompress(self):
        raw = token_bytes(1_000)
        compressed = zstd.compress(raw)
        decompressed = zstd.decompress(compressed)
        self.assertEqual(decompressed, raw)

    def test_zstdfile(self):
        raw = token_bytes(1_000)
        fobj = BytesIO()
        with zstd.ZstdFile(fobj, "w") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.ZstdFile(fobj) as fzstd:
            self.assertEqual(fzstd.read(), raw)
        self.assertTrue(fobj.tell() > 0)

    def test_open(self):
        raw = token_bytes(1_000)
        fobj = BytesIO()
        with zstd.open(fobj, "w") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.open(fobj) as fzstd:
            self.assertEqual(fzstd.read(), raw)
        self.assertTrue(fobj.tell() > 0)

    def test_open_binary(self):
        raw = token_bytes(1_000)
        fobj = BytesIO()
        with zstd.open(fobj, "wb") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.open(fobj, "rb") as fzstd:
            self.assertEqual(fzstd.read(), raw)
        self.assertTrue(fobj.tell() > 0)

    def test_open_text(self):
        raw = token_urlsafe(1_000)
        fobj = BytesIO()
        with zstd.open(fobj, "wt") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.open(fobj, "rt") as fzstd:
            self.assertEqual(fzstd.read(), raw)
        self.assertTrue(fobj.tell() > 0)


if __name__ == "__main__":
    unittest.main()
