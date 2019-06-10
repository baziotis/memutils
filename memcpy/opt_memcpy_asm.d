extern(C) void opt_memcpy_asm(void *dst, const(void) *src, size_t n) {
    // ASM written by hand based on opt_memcpy.c
    // That hopefully should be faster than the GCC version.
    asm pure nothrow @nogc {
        mov    RDX, T.sizeof;
        cmp    RDX, 0x7f;
        jbe    L5;     // if (n < 128)
        mov    ECX, ESI;                       // save `src`
        and    ECX, 0x1f;                      // mod = src % 32
        je     L1;
        // if (mod) -> copy enough bytes to reach 32-byte alignment
        movdqu XMM0, [RSI];
        movdqu XMM1, [RSI+0x10];
        movdqu [RDI], XMM0;
        movdqu [RDI+0x10], XMM1;
        // %t0 = 32 - mod
        mov    RAX, 0x20;
        sub    RAX, RCX;
        //cdqe   ;
        // src += %t0
        add    RSI, RAX;
        // dst += %t0
        add    RDI, RAX;
        // n -= %t0
        sub    RDX, RAX;
    L1:
        cmp    RDX, 0x7f;
        jbe    L2;
    // if (n >= 128)
    align 16;
    L4:
        // Because of the above, (at least) the loads
        // are 32-byte aligned.
        vmovdqu YMM0, [RSI];
        vmovdqu YMM1, [RSI+0x20];
        vmovdqu YMM2, [RSI+0x40];
        vmovdqu YMM3, [RSI+0x60];
        vmovdqu [RDI], YMM0;
        vmovdqu [RDI+0x20], YMM1;
        vmovdqu [RDI+0x40], YMM2;
        vmovdqu [RDI+0x60], YMM3;
        // src += 128;
        add    RSI, 128;
        // dst += 128;
        add    RDI, 128;
        // n -= 128;
        sub    RDX, 128;
        // if (n >= 128) loop
        cmp    RDX, 128;
        jge    L4;
    L2:
        test   RDX, RDX;
        je     L3;
        // if (n != 0)  -> copy the remaining <= 128 bytes
        lea    RSI, [RSI-128+RDX];
        lea    RDI, [RDI-128+RDX];
        vmovdqu YMM0, [RSI];
        vmovdqu YMM1, [RSI+0x20];
        vmovdqu YMM2, [RSI+0x40];
        vmovdqu YMM3, [RSI+0x60];
        vmovdqu [RDI], YMM0;
        vmovdqu [RDI+0x20], YMM1;
        vmovdqu [RDI+0x40], YMM2;
        vmovdqu [RDI+0x60], YMM3;
    L3:
        vzeroupper;
    }
    return;
    asm pure nothrow @nogc {
    L5:
        // if (n < 128)
        vmovdqu YMM0, [RSI];
        vmovdqu YMM1, [RSI+0x20];
        vmovdqu [RDI], YMM0;
        vmovdqu [RDI+0x20], YMM1;
        sub     RDX, 0x40;
        add     RSI, RDX;
        add     RDI, RDX;
        vmovdqu YMM0, [RSI];
        vmovdqu YMM1, [RSI+0x20];
        vmovdqu [RDI], YMM0;
        vmovdqu [RDI+0x20], YMM1;
    }
    return;
}
