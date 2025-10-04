import platform
import sys
import sysconfig
from distutils.unixccompiler import UnixCCompiler
from pathlib import Path

from setuptools import Extension, setup

# create a LICENSE_zstd.txt file
# wheels distributions needs to ship the license of the zstd library
ROOT_PATH = Path(__file__).parent.absolute()
with (ROOT_PATH / "LICENSE_zstd.txt").open("w") as f:
    f.write(
        "Depending on how it is build, this package may distribute the zstd library,\n"
        "partially or in its integrality, in source or binary form.\n\n"
        "Its license is reproduced below.\n\n"
        "---\n\n"
    )
    f.write((ROOT_PATH / "src" / "c" / "zstd" / "LICENSE").read_text())


UnixCCompiler.src_extensions.append(".S")


_PLATFORM_IS_WIN = sysconfig.get_platform().startswith("win")
_USE_CFFI = platform.python_implementation() == "PyPy"
try:
    sys.argv.remove("--system-zstd")
except ValueError:
    _SYSTEM_ZSTD = False
else:
    _SYSTEM_ZSTD = True


def locate_sources(*sub_paths):
    extensions = "cC" if _PLATFORM_IS_WIN else "cCsS"
    yield from map(str, Path(*sub_paths).rglob(f"*.[{extensions}]"))


def build_extension():
    kwargs = dict(
        sources=[],
        include_dirs=[],
        libraries=[],
        extra_compile_args=[],
        extra_link_args=[],
        define_macros=[
            ("ZSTD_MULTITHREAD", None),  # enable multithreading support
        ],
    )

    if _PLATFORM_IS_WIN:
        kwargs["extra_compile_args"] += ["/Ob3", "/GF", "/Gy"]
    else:
        kwargs["extra_compile_args"] += ["-g0", "-flto"]
        kwargs["extra_link_args"] += ["-g0", "-flto"]

    if _SYSTEM_ZSTD:
        kwargs["libraries"].append("zstd")
    else:
        kwargs["sources"] += [
            *locate_sources("src", "c", "zstd", "lib", "common"),
            *locate_sources("src", "c", "zstd", "lib", "compress"),
            *locate_sources("src", "c", "zstd", "lib", "decompress"),
            *locate_sources("src", "c", "zstd", "lib", "dictBuilder"),
        ]
        kwargs["include_dirs"] += [
            "src/c/zstd/lib",
            "src/c/zstd/lib/common",
            "src/c/zstd/lib/dictBuilder",
        ]

    if _USE_CFFI:
        import cffi

        ffibuilder = cffi.FFI()
        ffibuilder.cdef((ROOT_PATH / "src" / "c" / "cffi" / "cdef.h").read_text())
        ffibuilder.set_source(
            source=(ROOT_PATH / "src" / "c" / "cffi" / "source.c").read_text(),
            module_name="backports.zstd._zstd_cffi",
            **kwargs,
        )
        return ffibuilder.distutils_extension()

    else:
        kwargs["sources"] += [
            *locate_sources("src", "c", "compression_zstd"),
            *locate_sources("src", "c", "compat"),
        ]
        kwargs["include_dirs"] += [
            "src/c/compat",
            "src/c/compression_zstd",
            "src/c/compression_zstd/clinic",
            "src/c/pythoncapi-compat",
        ]
        return Extension(
            name="backports.zstd._zstd",
            **kwargs,
        )


setup(ext_modules=[build_extension()])
