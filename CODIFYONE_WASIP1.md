# CodifyOne mrustc native WASIp1 port

This fork is the native build-time companion for CodifyOne's on-device Rust
toolchain. It keeps mrustc's C backend and adds the pieces needed to emit
`wasm32-wasip1` code with wasi-sdk.

## Changes in this fork

- accepts Rust's `#[link(wasm_import_module = "...")]` attribute without
  turning it into a native linker library;
- includes a `wasm32-wasip1.toml` mrustc target;
- includes minimal `setjmp` and unwind shims for the current experimental
  WASIp1 standard-library build.

The shims trap when unwinding is attempted. Build Rust code with panic abort;
this is not an exception-unwinding implementation.

## Build

Install wasi-sdk and expose its root as `WASI_SDK_PATH`, then run:

```sh
make -j8
WASI_SDK_PATH=/path/to/wasi-sdk ./scripts/prepare-codifyone-wasip1.sh
```

Use the target file with either compiler frontend:

```sh
MRUSTC_TARGET_VER=1.90 ./bin/mrustc \
  --target ./wasm32-wasip1.toml \
  -L output-wasip1 path/to/main.rs -o app.wasm

MRUSTC_TARGET_VER=1.90 ./bin/minicargo \
  --target ./wasm32-wasip1.toml \
  -L output-wasip1 path/to/Cargo.toml
```

`output-wasip1` is intentionally not versioned. Populate it with the matching
Rust core/alloc/std artifacts before compiling applications. The CodifyOne app
ships the validated runtime artifacts separately.

The app-facing dispatcher lives in `codifyone/dispatcher.cpp`. It validates
ordinary CLI errors before entering mrustc/minicargo because this Wasmi build
uses abort-on-exception semantics: an uncaught C++ error would otherwise be
reported as a WebAssembly `unreachable` trap. Relink a prepared WASIp1 object
tree with `scripts/link-codifyone-toolchain.sh`.
