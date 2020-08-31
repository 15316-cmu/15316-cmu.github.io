
#ifndef _SAFEMEM_H
#define _SAFEMEM_H

#if (defined(__MACH__))
    #define SANDBOX_SECTION __attribute__((section ("__DATA,__sandbox"))) __attribute__((aligned (1)))
#elif (defined(__ELF__))
#define SANDBOX_SECTION __attribute__((section ("__sandbox"))) __attribute__((aligned (1)))
#endif

// ~16mb of heap for the interpreter
#define MAXHEAP 0xFFFFD3

void safe_free(void *ap);
void *safe_malloc(size_t nbytes);
void *safe_calloc(size_t n, size_t size);

#endif
