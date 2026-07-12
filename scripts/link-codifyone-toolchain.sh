#!/bin/sh
set -eu

: "${WASI_SDK:?set WASI_SDK to the wasi-sdk root}"
: "${WASM_OBJECT_ROOT:?set WASM_OBJECT_ROOT to the WASIp1 mrustc object tree}"

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR=${BUILD_DIR:-/tmp/mrustc-wasi-build}
OUTPUT=${OUTPUT:-$BUILD_DIR/rust-toolchain.wasm}
CXX="$WASI_SDK/bin/clang++"
AR="$WASI_SDK/bin/llvm-ar"
WASI_CXX_INCLUDE="$WASI_SDK/share/wasi-sysroot/include/wasm32-wasip1/noeh/c++/v1"

mkdir -p "$BUILD_DIR"
"$CXX" -O2 -fno-exceptions -fno-rtti -I "$WASI_CXX_INCLUDE" \
    -c "$ROOT/codifyone/dispatcher.cpp" -o "$BUILD_DIR/dispatcher.o"

rm -f "$BUILD_DIR/mrustc-all.a"
find "$WASM_OBJECT_ROOT/.obj" "$WASM_OBJECT_ROOT/tools/minicargo/.obj" \
    "$WASM_OBJECT_ROOT/tools/common/.obj" -type f -name '*.o' \
    ! -path "$WASM_OBJECT_ROOT/.obj/main.o" \
    ! -path "$WASM_OBJECT_ROOT/tools/common/.obj/jobserver.o" \
    -exec "$AR" rcs "$BUILD_DIR/mrustc-all.a" '{}' +

# mrustc_main.o is main.cpp compiled with main=mrustc_main. The two small
# stubs keep exception and jobserver failure paths deterministic under Wasmi.
"$CXX" -O2 -fno-exceptions -fno-rtti \
    "$BUILD_DIR/dispatcher.o" "$BUILD_DIR/mrustc_main.o" \
    "$BUILD_DIR/exception_stubs.o" "$BUILD_DIR/jobserver_stub.o" \
    -Wl,--whole-archive "$BUILD_DIR/mrustc-all.a" -Wl,--no-whole-archive \
    "$BUILD_DIR/libz.a" -lwasi-emulated-process-clocks \
    -Wl,--allow-undefined -Wl,--initial-memory=268435456 \
    -Wl,--max-memory=1073741824 -Wl,-z,stack-size=16777216 \
    -o "$OUTPUT"

# The mrustc object tree is intentionally built with debug information for
# diagnosis, but shipping those custom sections inflates the module from about
# 11 MiB to over 100 MiB and slows interpreter startup considerably.
"$WASI_SDK/bin/llvm-strip" --strip-debug "$OUTPUT"

echo "linked $OUTPUT"
