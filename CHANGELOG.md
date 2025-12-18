# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/), and this project
adheres to [Semantic Versioning](https://semver.org/).

## Unreleased

### :bug: Fixes

- Fix conflict on `__init__.py` file on `backports` root module by transforming
  `backports.zstd` into an implicit namespace package (see PEP-420)
- Raise an exception at both build and runtime when using an unsupported Python version,
  instead of crashing with a segmentation fault at runtime (in the rare cases where
  `backports.zstd` was installed despite the `requires-python` marker)

## [1.2.0] - 2025-12-06

[1.2.0]: https://github.com/rogdham/backports.zstd/releases/tag/v1.2.0

### :rocket: Added

- Update code with CPython 3.14.2 version
- Build wheels for riscv64

## [1.1.0] - 2025-11-23

[1.1.0]: https://github.com/rogdham/backports.zstd/releases/tag/v1.1.0

### :rocket: Added

- Shorten import time by lazy loading the `register_shutil` function

### :bug: Fixes

- Fix assertion on Python 3.13 when build with `DEBUG`

## [1.0.0] - 2025-10-10

[1.0.0]: https://github.com/rogdham/backports.zstd/releases/tag/v1.0.0

### :rocket: Added

- Update code with CPython 3.14.0 version
- Update type hints with typeshed `aa5202465`
- Update `pythoncapi-compat` dependency
- Allow to use `libzstd` present on the system with the `--system-zstd` build backend
  argument
- Check the `libzstd` version during build and at runtime

### :bug: Fixes

- Fix import order issue by importing the `tarfile` and `zipfile` modules only when
  needed.

## [0.5.0] - 2025-08-17

[0.5.0]: https://github.com/rogdham/backports.zstd/releases/tag/v0.5.0

### :boom: Breaking changes

- Update code with CPython 3.14.0 release candidate 2 version

### :rocket: Added

- Support for PyPy: Python 3.10 and 3.11
- Update type hints with typeshed `554701e9b`

## [0.4.0] - 2025-08-03

[0.4.0]: https://github.com/rogdham/backports.zstd/releases/tag/v0.4.0

### :boom: Breaking changes

- Update code with CPython 3.14.0 release candidate 1 version

### :rocket: Added

- Integration with the `shutil` module

## [0.3.0] - 2025-07-17

[0.3.0]: https://github.com/rogdham/backports.zstd/releases/tag/v0.3.0

### :boom: Breaking changes

- Update code with CPython 3.14.0 beta 4 version

### :rocket: Added

- Backport `tarfile` module
- Backport `zipfile` module
- Support for CPython 3.9
- Support for CPython 3.13 free-threaded

## [0.2.0] - 2025-06-08

[0.2.0]: https://github.com/rogdham/backports.zstd/releases/tag/v0.2.0

### :boom: Breaking changes

- Update code with CPython 3.14.0 beta 2 version

### :rocket: Added

- Support for CPython 3.10, 3.11 and 3.12
- Enable zstd multithreading support
- Add type hints
- Build wheels for more architectures

## [0.1.0] - 2025-05-08

[0.1.0]: https://github.com/rogdham/backports.zstd/releases/tag/v0.1.0

Initial public release, with support for Python 3.13 only.
