#!/bin/bash

set -Exeuo pipefail

#
# Usage
#
# if code is based on 3.14.0 and want to change to 3.14.1:
#     $0 v3.14.0 v3.14.1
# if you just want to compute the diffs with an upstream version:
#     $0 v3.14.0 v3.14.0
# note that both args are any valid git revision, but tags work fine
#

cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.."

# load CPYTHON_CLONE_PATH
set -a
source sync/.env
set +a

# make sure local git repo is clean
test -z "$(git status --porcelain | tee /dev/stderr)"

# fetch CPython tags
src_rev="$1"
dst_rev="$2"
git -C "$CPYTHON_CLONE_PATH" fetch
git -C "$CPYTHON_CLONE_PATH" show --no-patch "$src_rev"
git -C "$CPYTHON_CLONE_PATH" show --no-patch "$dst_rev"

# cleanup
patch_dir="sync/cpython"
rm -rf "$patch_dir"
find . -name '*.orig' -delete
find . -name '*.rej' -delete

# perform the diff & patch
set +x
while read -r src_path dst_path; do
	echo ">>>> $src_path"
	mkdir -p "$patch_dir/$(dirname "$dst_path")"
	patch_file="$patch_dir/$dst_path.patch"
	git -C "$CPYTHON_CLONE_PATH" show "$src_rev:$src_path" >"$dst_path"
	git diff -R --output="$patch_file" "$dst_path"
	git -C "$CPYTHON_CLONE_PATH" show "$dst_rev:$src_path" >"$dst_path"
	if [ -s "$patch_file" ]; then
		patch -i "$patch_file" -p 1 || true
	else
		rm "$patch_file"
	fi
done <<EOF
Include/internal/pycore_blocks_output_buffer.h                                      src/c/compat/pycore_blocks_output_buffer.h
Modules/_zstd/zstddict.h                                                            src/c/compression_zstd/zstddict.h
Modules/_zstd/_zstdmodule.c                                                         src/c/compression_zstd/_zstdmodule.c
Modules/_zstd/zstddict.c                                                            src/c/compression_zstd/zstddict.c
Modules/_zstd/decompressor.c                                                        src/c/compression_zstd/decompressor.c
Modules/_zstd/buffer.h                                                              src/c/compression_zstd/buffer.h
Modules/_zstd/_zstdmodule.h                                                         src/c/compression_zstd/_zstdmodule.h
Modules/_zstd/compressor.c                                                          src/c/compression_zstd/compressor.c
Modules/_zstd/clinic/compressor.c.h                                                 src/c/compression_zstd/clinic/compressor.c.h
Modules/_zstd/clinic/zstddict.c.h                                                   src/c/compression_zstd/clinic/zstddict.c.h
Modules/_zstd/clinic/_zstdmodule.c.h                                                src/c/compression_zstd/clinic/_zstdmodule.c.h
Modules/_zstd/clinic/decompressor.c.h                                               src/c/compression_zstd/clinic/decompressor.c.h
Lib/compression/_common/_streams.py                                                 src/python/backports/zstd/_streams.py
Lib/compression/zstd/__init__.py                                                    src/python/backports/zstd/__init__.py
Lib/compression/zstd/_zstdfile.py                                                   src/python/backports/zstd/_zstdfile.py
Lib/tarfile.py                                                                      src/python/backports/zstd/tarfile.py
Lib/zipfile/__init__.py                                                             src/python/backports/zstd/zipfile/__init__.py
Lib/zipfile/__main__.py                                                             src/python/backports/zstd/zipfile/__main__.py
Lib/zipfile/_path/__init__.py                                                       src/python/backports/zstd/zipfile/_path/__init__.py
Lib/zipfile/_path/glob.py                                                           src/python/backports/zstd/zipfile/_path/glob.py
Lib/test/__init__.py                                                                tests/test/__init__.py
Lib/test/tokenizedata/__init__.py                                                   tests/test/tokenizedata/__init__.py
Lib/test/tokenizedata/tokenize_tests-no-coding-cookie-and-utf8-bom-sig-only.txt     tests/test/tokenizedata/tokenize_tests-no-coding-cookie-and-utf8-bom-sig-only.txt
Lib/test/tokenizedata/tokenize_tests.txt                                            tests/test/tokenizedata/tokenize_tests.txt
Lib/test/tokenizedata/badsyntax_3131.py                                             tests/test/tokenizedata/badsyntax_3131.py
Lib/test/support/__init__.py                                                        tests/test/support/__init__.py
Lib/test/support/script_helper.py                                                   tests/test/support/script_helper.py
Lib/test/support/warnings_helper.py                                                 tests/test/support/warnings_helper.py
Lib/test/support/threading_helper.py                                                tests/test/support/threading_helper.py
Lib/test/support/import_helper.py                                                   tests/test/support/import_helper.py
Lib/test/support/os_helper.py                                                       tests/test/support/os_helper.py
Lib/test/test_zipfile/__init__.py                                                   tests/test/test_zipfile/__init__.py
Lib/test/test_zipfile/__main__.py                                                   tests/test/test_zipfile/__main__.py
Lib/test/test_zipfile/test_core.py                                                  tests/test/test_zipfile/test_core.py
Lib/test/test_zipfile/_path/__init__.py                                             tests/test/test_zipfile/_path/__init__.py
Lib/test/test_zipfile/_path/write-alpharep.py                                       tests/test/test_zipfile/_path/write-alpharep.py
Lib/test/test_zipfile/_path/_test_params.py                                         tests/test/test_zipfile/_path/_test_params.py
Lib/test/test_zipfile/_path/test_complexity.py                                      tests/test/test_zipfile/_path/test_complexity.py
Lib/test/test_zipfile/_path/_itertools.py                                           tests/test/test_zipfile/_path/_itertools.py
Lib/test/test_zipfile/_path/_functools.py                                           tests/test/test_zipfile/_path/_functools.py
Lib/test/test_zipfile/_path/_support.py                                             tests/test/test_zipfile/_path/_support.py
Lib/test/test_zipfile/_path/test_path.py                                            tests/test/test_zipfile/_path/test_path.py
Lib/test/archivetestdata/testtar.tar.xz                                             tests/test/archivetestdata/testtar.tar.xz
Lib/test/archivetestdata/testtar.tar                                                tests/test/archivetestdata/testtar.tar
Lib/test/archivetestdata/exe_with_zip                                               tests/test/archivetestdata/exe_with_zip
Lib/test/archivetestdata/recursion.tar                                              tests/test/archivetestdata/recursion.tar
Lib/test/archivetestdata/zip_cp437_header.zip                                       tests/test/archivetestdata/zip_cp437_header.zip
Lib/test/archivetestdata/zipdir_backslash.zip                                       tests/test/archivetestdata/zipdir_backslash.zip
Lib/test/archivetestdata/zipdir.zip                                                 tests/test/archivetestdata/zipdir.zip
Lib/test/archivetestdata/exe_with_z64                                               tests/test/archivetestdata/exe_with_z64
Lib/test/test_zstd.py                                                               tests/test/test_zstd.py
Lib/test/archiver_tests.py                                                          tests/test/archiver_tests.py
Lib/test/test_tarfile.py                                                            tests/test/test_tarfile.py
EOF
find "$patch_dir" -type d -empty -delete

echo ">>> Rejected patches"
find . -name '*.rej'

echo ">>> Also check shutil diffs manually"
git -C "$CPYTHON_CLONE_PATH" diff "$src_rev...$dst_rev" "Lib/shutil.py"

echo ">>> Script finished, but diffs needs to be reviewed manually"
