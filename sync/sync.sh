#!/bin/bash

set -Exeuo pipefail

#
# Usage: $0 python <revision_source> <revision_destination>
#
# if code is based on 3.14.0 and want to change to 3.14.1:
#     $0 cpython v3.14.0 v3.14.1
# if you just want to compute the diffs with an upstream version:
#     $0 cpython v3.14.0 v3.14.0
# note that both args are any valid git revision, but tags work fine
#

cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.."

# load CLONE_PATH, etc.
set -a
source sync/.env
set +a

# make sure local git repo is clean
test -z "$(git status --porcelain | tee /dev/stderr)"

# arguments
kind="$1"
[ -f "sync/$kind.json" ]
env_var_clone_path="${kind^^}_CLONE_PATH"
CLONE_PATH="${!env_var_clone_path}"

# fetch CPython tags
src_rev="$2"
dst_rev="$3"
git -C "$CLONE_PATH" fetch
git -C "$CLONE_PATH" show --no-patch "$src_rev" --
git -C "$CLONE_PATH" show --no-patch "$dst_rev" --

# cleanup
patch_dir="sync/$kind"
rm -rf "$patch_dir"
find . -name '*.orig' -delete
find . -name '*.rej' -delete

# perform the diff & patch
set +x
jq -r 'to_entries[] | (.key+" "+(.value|tojson))' "sync/$kind.json" | while read -r dst_path src_paths; do
	echo ">>>> $dst_path"
	mkdir -p "$patch_dir/$(dirname "$dst_path")"
	patch_file="$patch_dir/$dst_path.patch"
	# override with src revision
	truncate -s 0 "$dst_path"
	echo "$src_paths" | jq -r '.[]' | while read -r src_path; do
		git -C "$CLONE_PATH" show "$src_rev:$src_path" >>"$dst_path"
	done
	# create patch file
	git diff -R --output="$patch_file" "$dst_path"
	# override with dst revision
	truncate -s 0 "$dst_path"
	echo "$src_paths" | jq -r '.[]' | while read -r src_path; do
		git -C "$CLONE_PATH" show "$dst_rev:$src_path" >>"$dst_path"
	done
	# apply patch file
	if [ -s "$patch_file" ]; then
		patch -i "$patch_file" -p 1 || true
	else
		rm "$patch_file"
	fi
done

find "$patch_dir" -type d -empty -delete

echo ">>> Rejected patches"
find . -name '*.rej'

if [ "$kind" == "cpython" ]; then
	echo ">>> Also check shutil diffs manually"
	git -C "$CLONE_PATH" diff "$src_rev...$dst_rev" "Lib/shutil.py"
fi

echo ">>> Script finished, but diffs needs to be reviewed manually"
