#include <immintrin.h>
#include <stdint.h>

inline
void mov16_u(uint8_t *dst, const uint8_t *src) {
    __m128i xmm0;
    asm volatile (
        "movdqu (%[src]), %[xmm0]\n\t"
        "movdqu %[xmm0], (%[dst])\n\t"
        : [xmm0] "=x" (xmm0)
        : [src] "r" (src),
          [dst] "r"(dst)
        : "memory"
    );
}

inline
void mov32_u(uint8_t *dst, const uint8_t *src)
{
    __m128i xmm0, xmm1;
    asm volatile (
        "movdqu (%[src]), %[xmm0]\n\t"
        "movdqu 16(%[src]), %[xmm1]\n\t"
        "movdqu %[xmm0], (%[dst])\n\t"
        "movdqu %[xmm1], 16(%[dst])\n\t"
        : [xmm0] "=x" (xmm0),
          [xmm1] "=x" (xmm1)
        : [src] "r" (src),
          [dst] "r"(dst)
        : "memory"
    );
}

inline void
mov64_u(uint8_t *dst, const uint8_t *src)
{
#ifdef INTEL_PROC
    __m256i ymm0, ymm1;
    asm volatile (
        "vmovdqu (%[src]), %[ymm0]\n\t"
        "vmovdqu 32(%[src]), %[ymm1]\n\t"
        "vmovdqu %[ymm0], (%[dst])\n\t"
        "vmovdqu %[ymm1], 32(%[dst])\n\t"
        : [ymm0] "=x" (ymm0),
          [ymm1] "=x" (ymm1)
        : [src] "r" (src),
          [dst] "r"(dst)
        : "memory"
    );
#else
    __m128i xmm0, xmm1, xmm2, xmm3;
    asm volatile (
        "movdqu (%[src]), %[xmm0]\n\t"
        "movdqu 16(%[src]), %[xmm1]\n\t"
        "movdqu 32(%[src]), %[xmm2]\n\t"
        "movdqu 48(%[src]), %[xmm3]\n\t"
        "movdqu %[xmm0], (%[dst])\n\t"
        "movdqu %[xmm1], 16(%[dst])\n\t"
        "movdqu %[xmm2], 32(%[dst])\n\t"
        "movdqu %[xmm3], 48(%[dst])\n\t"
        : [xmm0] "=x" (xmm0),
          [xmm1] "=x" (xmm1),
          [xmm2] "=x" (xmm2),
          [xmm3] "=x" (xmm3)
        : [src] "r" (src),
          [dst] "r"(dst)
        : "memory"
    );
#endif
}

inline
void mov128_u(uint8_t *dst, const uint8_t *src)
{
#ifdef INTEL_PROC
    __m256i ymm0, ymm1, ymm2, ymm3;
    asm volatile (
        "vmovdqu (%[src]), %[ymm0]\n\t"
        "vmovdqu 32(%[src]), %[ymm1]\n\t"
        "vmovdqu 64(%[src]), %[ymm2]\n\t"
        "vmovdqu 96(%[src]), %[ymm3]\n\t"
        "vmovdqu %[ymm0], (%[dst])\n\t"
        "vmovdqu %[ymm1], 32(%[dst])\n\t"
        "vmovdqu %[ymm2], 64(%[dst])\n\t"
        "vmovdqu %[ymm3], 96(%[dst])\n\t"
        : [ymm0] "=x" (ymm0),
          [ymm1] "=x" (ymm1),
          [ymm2] "=x" (ymm2),
          [ymm3] "=x" (ymm3)
        : [src] "r" (src),
          [dst] "r"(dst)
        : "memory"
    );
#else
    __m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
    asm volatile (
        "movdqu (%[src]), %[xmm0]\n\t"
        "movdqu 16(%[src]), %[xmm1]\n\t"
        "movdqu 32(%[src]), %[xmm2]\n\t"
        "movdqu 48(%[src]), %[xmm3]\n\t"
        "movdqu 64(%[src]), %[xmm4]\n\t"
        "movdqu 80(%[src]), %[xmm5]\n\t"
        "movdqu 96(%[src]), %[xmm6]\n\t"
        "movdqu 112(%[src]), %[xmm7]\n\t"
        "movdqu %[xmm0], (%[dst])\n\t"
        "movdqu %[xmm1], 16(%[dst])\n\t"
        "movdqu %[xmm2], 32(%[dst])\n\t"
        "movdqu %[xmm3], 48(%[dst])\n\t"
        "movdqu %[xmm4], 64(%[dst])\n\t"
        "movdqu %[xmm5], 80(%[dst])\n\t"
        "movdqu %[xmm6], 96(%[dst])\n\t"
        "movdqu %[xmm7], 112(%[dst])\n\t"
        : [xmm0] "=x" (xmm0),
          [xmm1] "=x" (xmm1),
          [xmm2] "=x" (xmm2),
          [xmm3] "=x" (xmm3),
          [xmm4] "=x" (xmm4),
          [xmm5] "=x" (xmm5),
          [xmm6] "=x" (xmm6),
          [xmm7] "=x" (xmm7)
        : [src] "r" (src),
          [dst] "r"(dst)
        : "memory"
    );
#endif
}

inline void
mov128_up(uint8_t *dst, const uint8_t *src)
{
    asm volatile ("prefetcht0 %[p]" : : [p] "m" (*(const volatile char *)(src+0x1a0)));
    asm volatile ("prefetcht0 %[p]" : : [p] "m" (*(const volatile char *)(src+0x260)));
    mov128_u(dst, src);
}

void opt_memcpy(void *dst, const void *src, size_t n)
{
    // IMPORTANT(stefanos): For now, n > 64;
    //assert(n > 64);
    if (n < 128) {
        // IMPORTANT(stefanos): DO NOT DO THIS! It will overwrite previous data,
        // as n < 128.
        //mov128_u((uint8_t *)dst - 128 + n, (const uint8_t *)src - 128 + n);
        //
        mov64_u(dst, src);
        mov64_u(dst - 64 + n, src - 64 + n);
        return;
    }

    // Align to 32-byte boundary. Move what is needed.
    int mod = (uintptr_t)src % 32;
    if (mod) {
        mov32_u(dst, src);
        src += 32 - mod;
        dst += 32 - mod;
        n -= 32 - mod;
    }



#ifdef INTEL_PROC
    for ( ; n >= 128; n -= 128) {
        mov128_u(dst, src);
        dst = dst + 128;
        src = src + 128;
    }
#endif
    if (n >= 20000) {
        for ( ; n >= 128; n -= 128) {
            mov128_up(dst, src);
            mov128_u(dst, src);
            dst = dst + 128;
            src = src + 128;
        }
    } else {
        for ( ; n >= 128; n -= 128) {
            mov128_u(dst, src);
            dst = dst + 128;
            src = src + 128;
        }
    }

#ifdef INTEL_PROC

    // Copy remaining < 128 bytes.
    // NOTE(stefanos): We already have checked that the initial size is >= 128
    // to be here. So, we won't overwrite previous data.
    if (n != 0) {
        mov128_u(dst - 128 + n, src - 128 + n);
    }
#else

    // NOTE(stefanos): If you have GCC or Clang back-end, don't do yourself
    // the division with shifts etc. and make the code harder to read.
    // It's a division by a constant and it's trivial for an optimizing compiler
    // to do it itself. What is not trivial though is to know that the loop is
    // going to execute <= 3 times. You could write this code using a 3-case fallthourgh switch
    // fall-through, but again, it makes the code more complicated without
    // any significant win.

    // 64-byte portions.
    // <= 3 times loop.
    for (size_t i = n / 64; i; --i) {
        mov64_u(dst, src);
        n -= 64;
        dst = dst + 64;
        src = src + 64;
    }

    // 16-byte portions.
    // <= 3 times loop.
    for (size_t i = n / 16; i; --i) {
        mov16_u(dst, src);
        n -= 16;
        dst = dst + 16;
        src = src + 16;
    }

    // Copy remaining < 16 bytes.
    if (n != 0) {
        mov16_u(dst - 16 + n, src - 16 + n);
    }
#endif
}
