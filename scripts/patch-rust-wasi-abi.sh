#!/bin/sh
set -eu

if [ "$#" -ne 1 ] || [ ! -f "$1" ]; then
    echo "usage: $0 path/to/libwasi-*.rlib.o" >&2
    exit 2
fi

# mrustc's C backend emits Rust's canonical preview1 declarations while
# wasi-libc owns several identically named internal functions with older C
# signatures. wasm-ld otherwise resolves the calls to the incompatible libc
# definitions and synthesizes signature_mismatch traps. Keep the replacement
# the same byte length so this works directly on a wasm object file; the
# rust_wasi_abi.o shim then supplies these renamed symbols as real imports.
perl -0pi -e '
    s/__wasi_path_readlink/__rust_path_readlink/g;
    s/__wasi_path_filestat_set_times/__rust_path_filestat_set_times/g;
    s/__wasi_path_unlink_file/__rust_path_unlink_file/g;
    s/__wasi_path_link/__rust_path_link/g;
    s/__wasi_path_filestat_get/__rust_path_filestat_get/g;
    s/__wasi_proc_exit/__rust_proc_exit/g;
    s/__wasi_path_symlink/__rust_path_symlink/g;
    s/__wasi_path_rename/__rust_path_rename/g;
    s/__wasi_path_open/__rust_path_open/g;
    s/__wasi_path_create_directory/__rust_path_create_directory/g;
    s/__wasi_path_remove_directory/__rust_path_remove_directory/g;
' "$1"

echo "patched Rust preview1 ABI symbols in $1"
