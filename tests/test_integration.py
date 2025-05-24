import sys

if sys.version_info < (3, 14):
    from backports import zstd

    zstd.patch_tarfile()

else:
    from compression import zstd

import unittest
from io import BytesIO
from secrets import token_bytes, token_urlsafe
import tarfile
from tempfile import TemporaryDirectory
from pathlib import Path


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

    def test_tarfile(self):
        raw = token_bytes(1_000)
        raw_name = token_urlsafe(10)
        with TemporaryDirectory() as tmpfile:
            path = Path(tmpfile) / "archive.tar.zst"
            with tarfile.open(path, "w:zst") as tf:
                ti = tarfile.TarInfo(raw_name)
                ti.size = len(raw)
                tf.addfile(ti, BytesIO(raw))

            with tarfile.open(path) as tf:
                self.assertEqual(tf.getnames(), [raw_name])
                with tf.extractfile(raw_name) as fobj:
                    self.assertEqual(fobj.read(), raw)


if __name__ == "__main__":
    unittest.main()
