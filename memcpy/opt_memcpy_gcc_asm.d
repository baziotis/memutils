extern(C) void opt_memcpy_gcc_asm(void *dst, const(void) *src, size_t n) {
    // GCC-ASM of opt_memcpy.c compiled with -O3

    // With %t(some_number) I define conceptual temporary
    // variables that don't exist in the C source code.
    
    asm pure nothrow @nogc {
        mov    RSI, src;
        mov    RDI, dst;
        mov    RDX, n;
        cmp    RDX, 0x7f;
        jbe    L5;     // if (n < 128)
        mov    ECX, ESI;                       // save `src`
        and    ECX, 0x1f;                      // mod = src % 32
        jne    L6;

    L4:
        lea    RAX, [RDX-0x80];
        and    RAX, 0xffffffffffffff80;
        lea    R8,[RAX+0x80];
        mov    RAX, RSI;
        lea    RCX, [RDI+R8];
        nop     ;
    align 16;
    L7:
        // Because of the above, (at least) the loads
        // are 32-byte aligned.
        vmovdqu YMM0, [RAX];
        vmovdqu YMM1, [RAX+0x20];
        vmovdqu YMM2, [RAX+0x40];
        vmovdqu YMM3, [RAX+0x60];
        vmovdqu [RDI], YMM0;
        vmovdqu [RDI+0x20], YMM1;
        vmovdqu [RDI+0x40], YMM2;
        vmovdqu [RDI+0x60], YMM3;
        sub    RDI, 0xffffffffffffff80;
        sub    RAX, 0xffffffffffffff80;
        cmp    RDI, RCX;
        jne    L7;
        add    RSI, R8;
        and    EDX, 0x7f;
        jne    L2;
        vzeroupper;
    }
    return;

    asm pure nothrow @nogc {
    L6:
        // if (mod) -> copy enough bytes to reach 32-byte alignment
        movdqu XMM0, [RSI];
        movdqu XMM1, [RSI+0x10];
        movdqu [RDI], XMM0;
        movdqu [RDI+0x10], XMM1;
        // %t0 = 32 - mod
        mov    EAX, 0x20;
        sub    EAX, ECX;
        cdqe   ;
        // n -= %t0
        sub    RDX, RAX;
        // src += %t0
        add    RSI, RAX;
        // dst += %t0
        add    RDI, RAX;
        cmp    RDX, 0x7f;
        ja L4;
    L2:
        add    RDX, 0xffffffffffffff80;
        add    RSI, RDX;
        add    RDI, RDX;
        // if (n != 0)  -> copy the remaining <= 128 bytes
        vmovdqu YMM0, [RSI];
        vmovdqu YMM1, [RSI+0x20];
        vmovdqu YMM2, [RSI+0x40];
        vmovdqu YMM3, [RSI+0x60];
        vmovdqu [RDI], YMM0;
        vmovdqu [RDI+0x20], YMM1;
        vmovdqu [RDI+0x40], YMM2;
        vmovdqu [RDI+0x60], YMM3;
        vzeroupper;
    }
    return;
    asm pure nothrow @nogc {
    align 16;
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
        vzeroupper;
    }
    return;
}
