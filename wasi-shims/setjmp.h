#ifndef CODIFYONE_WASI_SETJMP_H
#define CODIFYONE_WASI_SETJMP_H
#include <stdint.h>
typedef uintptr_t jmp_buf[32];
typedef jmp_buf sigjmp_buf;
#define setjmp(env) (0)
#define sigsetjmp(env, savesigs) (0)
__attribute__((noreturn)) static inline void longjmp(jmp_buf env, int value) {
    (void)env; (void)value; __builtin_trap();
}
__attribute__((noreturn)) static inline void siglongjmp(sigjmp_buf env, int value) {
    (void)env; (void)value; __builtin_trap();
}
#endif
