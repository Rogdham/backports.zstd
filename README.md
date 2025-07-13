<div align="center" size="15px">

# backports.zstd

Backport of [PEP-784 “adding Zstandard to the standard library”][PEP-784]

[![GitHub build status](https://img.shields.io/github/actions/workflow/status/rogdham/backports.zstd/build.yml?branch=master)](https://github.com/rogdham/backports.zstd/actions?query=branch:master)
[![Release on PyPI](https://img.shields.io/pypi/v/backports.zstd)](https://pypi.org/project/backports.zstd/)

---

[📖 PEP-784][PEP-784]&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;[📃 Changelog](./CHANGELOG.md)

[PEP-784]: https://peps.python.org/pep-0784/

</div>

---

## ⚠️ Work in progress

Not all features nor Python versions are supported at this time.

See the [🎯 Roadmap](https://github.com/Rogdham/backports.zstd/issues/2) for more
details on the status of this project.

---

## Install

Add the following dependency to your project:

```
backports.zstd ; python_version<'3.14'
```

## Usage

When importing a module needing Zstandard support, use a conditional import based on the
version of Python. See below for examples.

### zstd

```python
import sys

if sys.version_info >= (3, 14):
    from compression import zstd
else:
    from backports import zstd


# use the zstd module, for example:
zstd.compress(b"Hello, world!")
```

Refer to the [official Python documentation][doc-zstd] for usage of the module.

[doc-zstd]: https://docs.python.org/3.14/library/compression.zstd.html

### tarfile

```python
import sys

if sys.version_info >= (3, 14):
    import tarfile
else:
    from backports.zstd import tarfile


# use the tarfile module, for example:
with tarfile.open("archive.tar.zst") as tar:
    tar.list()
```

This `tarfile` modules is backported from Python 3.14 and includes Zstandard-specific
features such as: explicit modes for opening files (e.g. `r:zstd`), specific arguments
(e.g. `zstd_dict`)… refer to the [official Python documentation][doc-tarfile] for more
info.

[doc-tarfile]: https://docs.python.org/3.14/library/tarfile.html

Moreover, the CLI is available as well: `python -m backports.zstd.tarfile`.

### zipfile

```python
import sys

if sys.version_info >= (3, 14):
    import zipfile
else:
    from backports.zstd import zipfile


# use the zipfile module, for example:
with zipfile.ZipFile("archive.zip", "w") as zf:
    zf.writestr("hello.txt", "Hi!", zipfile.ZIP_ZSTANDARD)
```

This `zipfile` modules is backported from Python 3.14 and includes Zstandard-specific
features such as the constant `ZIP_ZSTANDARD` to be used for `compress_type`… refer to
the [official Python documentation][doc-zipfile] for more info.

[doc-zipfile]: https://docs.python.org/3.14/library/zipfile.html

Moreover, the CLI is available as well: `python -m backports.zstd.zipfile`.
