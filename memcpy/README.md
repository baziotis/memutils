# Optimized memcpy

This is a C / ASM implementation that resulted as I was experimenting for my GSoC project. For more info,
check [Dmemcpy](https://github.com/baziotis/Dmemcpy).

There I also discussed some optimization subjects.

Currently, there are 3 implementations. A C one and 2 ASM. The ASM implementations are written in D inline ASM as this
was the most related ASM I was using when I was experimening. But it should be trivial to convert it in any ASM.

The `opt_memcpy_gcc_asm.d` is the result ASM when `opt_memcpy.c` is compiled with `-O3`. I translated to ASM for direct
comparison with my ASM.

The `opt_memcpy_asm.d` is a hand-written ASM which is based on the same C source.

The C version is different for Intel and AMD, as there were significant differences on my benchmarks. The AMD one
uses only SSE.

The ASM versions are only the Intel version due to limited time.
