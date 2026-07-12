#!/bin/sh
set -eu

: "${WASI_SDK_PATH:?set WASI_SDK_PATH to the wasi-sdk installation root}"

clang="$WASI_SDK_PATH/bin/clang"
if [ ! -x "$clang" ]; then
    echo "wasi-sdk clang not found: $clang" >&2
    exit 1
fi

"$clang" \
    --target=wasm32-wasip1 \
    -c wasi-shims/unwind.c \
    -o wasi-shims/unwind.o

"$clang" \
    --target=wasm32-wasip1 \
    -c wasi-shims/rust_wasi_abi.c \
    -o wasi-shims/rust_wasi_abi.o

if [ -n "${RUST_WASI_OBJECT:-}" ]; then
    scripts/patch-rust-wasi-abi.sh "$RUST_WASI_OBJECT"
fi

echo "prepared wasm32-wasip1 target support"
