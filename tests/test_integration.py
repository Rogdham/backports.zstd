import shutil
import sys
import unittest
from io import BytesIO
from pathlib import Path
from secrets import token_bytes, token_urlsafe
from tempfile import TemporaryDirectory

if sys.version_info >= (3, 14):
    from compression import zstd
    import tarfile
    import zipfile
else:
    from backports import zstd
    from backports.zstd import register_shutil, tarfile, zipfile
    register_shutil()

if sys.version_info >= (3, 11):
    from typing import assert_type
else:
    assert_type = lambda *_: None

# these tests are simple checks for main use cases
# to make sure they work with the conditional import in 3.14 as well


class TestCompat(unittest.TestCase):
    def test_compress_decompress(self) -> None:
        raw = token_bytes(1_000)
        assert_type(raw, bytes)
        compressed = zstd.compress(raw)
        assert_type(compressed, bytes)
        decompressed = zstd.decompress(compressed)
        assert_type(decompressed, bytes)
        self.assertEqual(decompressed, raw)

    def test_zstdfile(self) -> None:
        raw = token_bytes(1_000)
        fobj = BytesIO()
        with zstd.ZstdFile(fobj, "w") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.ZstdFile(fobj) as fzstd:
            data = fzstd.read()
            assert_type(data, bytes)
            self.assertEqual(data, raw)
        self.assertTrue(fobj.tell() > 0)

    def test_open(self) -> None:
        raw = token_bytes(1_000)
        fobj = BytesIO()
        with zstd.open(fobj, "w") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.open(fobj) as fzstd:
            data = fzstd.read()
            assert_type(data, bytes)
            self.assertEqual(data, raw)
        self.assertTrue(fobj.tell() > 0)

    def test_open_binary(self) -> None:
        raw = token_bytes(1_000)
        fobj = BytesIO()
        with zstd.open(fobj, "wb") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.open(fobj, "rb") as fzstd:
            data = fzstd.read()
            assert_type(data, bytes)
            self.assertEqual(data, raw)
        self.assertTrue(fobj.tell() > 0)

    def test_open_text(self) -> None:
        raw = token_urlsafe(1_000)
        fobj = BytesIO()
        with zstd.open(fobj, "wt") as fzstd:
            fzstd.write(raw)
        self.assertTrue(fobj.tell() > 0)
        fobj.seek(0)
        with zstd.open(fobj, "rt") as fzstd:
            data = fzstd.read()
            assert_type(data, str)
            self.assertEqual(data, raw)
        self.assertTrue(fobj.tell() > 0)

    def test_tarfile(self) -> None:
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
                extracted = tf.extractfile(raw_name)
                assert extracted is not None  # for type checkers
                with extracted as fobj:
                    self.assertEqual(fobj.read(), raw)

            shutil.unpack_archive(path, tmpfile)
            self.assertEqual((Path(tmpfile) / raw_name).read_bytes(), raw)

    def test_zipfile(self) -> None:
        raw = token_bytes(1_000)
        raw_name = token_urlsafe(10)
        with TemporaryDirectory() as tmpfile:
            path = Path(tmpfile) / "archive.zip"
            with zipfile.ZipFile(path, "w") as zf:
                zf.writestr(raw_name, raw, zipfile.ZIP_ZSTANDARD)

            with zipfile.ZipFile(path) as zf:
                self.assertEqual(zf.namelist(), [raw_name])
                self.assertEqual(zf.read(raw_name), raw)

            shutil.unpack_archive(path, tmpfile)
            self.assertEqual((Path(tmpfile) / raw_name).read_bytes(), raw)

    def test_shutil_make_archive(self) -> None:
        raw = token_bytes(1_000)
        raw_name = token_urlsafe(10)
        with TemporaryDirectory() as tmpfile:
            path_src = Path(tmpfile) / "src"
            path_src.mkdir()
            (path_src / raw_name).write_bytes(raw)

            path_dst = Path(tmpfile) / "archive"
            shutil.make_archive(path_dst.as_posix(), "zstdtar", path_src)

            with path_dst.with_suffix(".tar.zst").open("rb") as f:
                self.assertEqual(f.read(4), bytes.fromhex("28 b5 2f fd"))


if __name__ == "__main__":
    unittest.main()
