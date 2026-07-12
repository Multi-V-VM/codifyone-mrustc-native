#include <stdint.h>

#define WASI_IMPORT(name) \
    __attribute__((import_module("wasi_snapshot_preview1"), import_name(name)))

WASI_IMPORT("path_readlink")
extern int32_t raw_path_readlink(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
WASI_IMPORT("path_filestat_set_times")
extern int32_t raw_path_filestat_set_times(
    int32_t, int32_t, int32_t, int32_t, int64_t, int64_t, int32_t);
WASI_IMPORT("path_unlink_file")
extern int32_t raw_path_unlink_file(int32_t, int32_t, int32_t);
WASI_IMPORT("path_link")
extern int32_t raw_path_link(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
WASI_IMPORT("path_filestat_get")
extern int32_t raw_path_filestat_get(int32_t, int32_t, int32_t, int32_t, int32_t);
WASI_IMPORT("proc_exit")
extern void raw_proc_exit(int32_t);
WASI_IMPORT("path_symlink")
extern int32_t raw_path_symlink(int32_t, int32_t, int32_t, int32_t, int32_t);
WASI_IMPORT("path_rename")
extern int32_t raw_path_rename(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
WASI_IMPORT("path_open")
extern int32_t raw_path_open(
    int32_t, int32_t, int32_t, int32_t, int32_t, int64_t, int64_t, int32_t, int32_t);
WASI_IMPORT("path_create_directory")
extern int32_t raw_path_create_directory(int32_t, int32_t, int32_t);
WASI_IMPORT("path_remove_directory")
extern int32_t raw_path_remove_directory(int32_t, int32_t, int32_t);

int32_t __rust_path_readlink(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f) {
    return raw_path_readlink(a, b, c, d, e, f);
}
int32_t __rust_path_filestat_set_times(
    int32_t a, int32_t b, int32_t c, int32_t d, int64_t e, int64_t f, int32_t g) {
    return raw_path_filestat_set_times(a, b, c, d, e, f, g);
}
int32_t __rust_path_unlink_file(int32_t a, int32_t b, int32_t c) {
    return raw_path_unlink_file(a, b, c);
}
int32_t __rust_path_link(
    int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g) {
    return raw_path_link(a, b, c, d, e, f, g);
}
int32_t __rust_path_filestat_get(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e) {
    return raw_path_filestat_get(a, b, c, d, e);
}
int32_t __rust_proc_exit(int32_t code) {
    raw_proc_exit(code);
    __builtin_unreachable();
}
int32_t __rust_path_symlink(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e) {
    return raw_path_symlink(a, b, c, d, e);
}
int32_t __rust_path_rename(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f) {
    return raw_path_rename(a, b, c, d, e, f);
}
int32_t __rust_path_open(
    int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
    int64_t f, int64_t g, int32_t h, int32_t i) {
    return raw_path_open(a, b, c, d, e, f, g, h, i);
}
int32_t __rust_path_create_directory(int32_t a, int32_t b, int32_t c) {
    return raw_path_create_directory(a, b, c);
}
int32_t __rust_path_remove_directory(int32_t a, int32_t b, int32_t c) {
    return raw_path_remove_directory(a, b, c);
}
