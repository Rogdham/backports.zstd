import shutil
import tempfile
import unittest
from io import BytesIO
from pathlib import Path
from secrets import token_bytes, token_urlsafe

from backports.zstd import register_shutil, tarfile, zipfile


class TestShutil(unittest.TestCase):
    def setUp(self):
        register_shutil()
        self.tmpdir = tempfile.TemporaryDirectory()
        self.tmppath = Path(self.tmpdir.name)
        (self.tmppath / "src").mkdir()
        (self.tmppath / "src" / "foo").write_text("bar")

    def tearDown(self):
        self.tmpdir.cleanup()

    def test_registration(self):
        self.assertTrue(("zstdtar", "zstd'ed tar-file") in shutil.get_archive_formats())
        self.assertTrue(("zip", "ZIP file") in shutil.get_archive_formats())
        self.assertTrue(
            ("zstdtar", [".tar.zst", ".tzst"], "zstd'ed tar-file")
            in shutil.get_unpack_formats()
        )
        self.assertTrue(("zip", [".zip"], "ZIP file") in shutil.get_unpack_formats())

    def test_make_archive_tar(self):
        shutil.make_archive((self.tmppath / "archive").as_posix(), "zstdtar", self.tmppath)
        with (self.tmppath / "archive.tar.zst").open("rb") as f:
            self.assertEqual(f.read(4), bytes.fromhex("28 b5 2f fd"))

    def test_unpack_archive_tar(self):
        archive_path = self.tmppath / "test.tar.zst"
        raw = token_bytes(1_000)
        raw_name = token_urlsafe(10)
        with tarfile.open(archive_path, "w:zst") as tf:
            ti = tarfile.TarInfo(raw_name)
            ti.size = len(raw)
            tf.addfile(ti, BytesIO(raw))
        out_path = self.tmppath / "test_tar"
        shutil.unpack_archive(archive_path, out_path)
        self.assertEqual((out_path / raw_name).read_bytes(), raw)

    def test_unpack_archive_zip(self):
        archive_path = self.tmppath / "test.zip"
        raw = token_bytes(1_000)
        raw_name = token_urlsafe(10)
        with zipfile.ZipFile(archive_path, "w") as zf:
            zf.writestr(raw_name, raw, zipfile.ZIP_ZSTANDARD)
        out_path = self.tmppath / "test_zip"
        shutil.unpack_archive(archive_path, out_path)
        self.assertEqual((out_path / raw_name).read_bytes(), raw)


if __name__ == "__main__":
    unittest.main()
