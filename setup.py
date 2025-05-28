from setuptools import setup, Extension
from distutils.unixccompiler import UnixCCompiler
from pathlib import Path
import sysconfig


UnixCCompiler.src_extensions.append(".S")

_PLATFORM_IS_WIN = sysconfig.get_platform().startswith("win")


def locate_sources(*sub_paths):
    extensions = "cC" if _PLATFORM_IS_WIN else "cCsS"
    yield from map(str, Path(*sub_paths).rglob(f"*.[{extensions}]"))


setup(
    ext_modules=[
        Extension(
            name="backports.zstd._zstd",
            sources=[
                *locate_sources("src", "c", "compression_zstd"),
                *locate_sources("src", "c", "compat"),
                *locate_sources("src", "c", "zstd", "lib", "common"),
                *locate_sources("src", "c", "zstd", "lib", "compress"),
                *locate_sources("src", "c", "zstd", "lib", "decompress"),
                *locate_sources("src", "c", "zstd", "lib", "dictBuilder"),
            ],
            include_dirs=[
                "src/c/compat",
                "src/c/compression_zstd",
                "src/c/compression_zstd/clinic",
                "src/c/pythoncapi-compat",
                "src/c/zstd/lib",
                "src/c/zstd/lib/common",
                "src/c/zstd/lib/dictBuilder",
            ],
            extra_compile_args=["/Ob3", "/GF", "/Gy"]
            if _PLATFORM_IS_WIN
            else ["-g0", "-flto"],
            extra_link_args=[] if _PLATFORM_IS_WIN else ["-g0", "-flto"],
            define_macros=[
                ("ZSTD_MULTITHREAD", None)  # enable multithreading support
            ],
        ),
    ],
)
