#pragma once

#define ALIF_ABS(x)	((x) < 0 ? -(x) : (x))
#define ALIF_MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define ALIF_MIN(a,b)   (((a) < (b)) ? (a) : (b))

#define ALIF_WCHARMASK(wc) ((wchar_t)((wc) & 0xffff)) 

#define ALIFSIZE_ROUND_UP(_n, _a) (((size_t)(_n) + (size_t)((_a) - 1)) & ~(size_t)((_a) - 1))

#define ALIF_ALIGN_DOWN(_p, _a) ((void *)((uintptr_t)(_p) & ~(uintptr_t)((_a) - 1)))

#if defined(__GNUC__) || defined(__clang__)
#  define ALIF_UNUSED(name) _unused_ ## name __attribute__((unused))
#elif defined(_MSC_VER)
// Disable warning C4100: unreferenced formal parameter,
// declare the parameter,
// restore old compiler warnings.
#  define ALIF_UNUSED(name) \
        __pragma(warning(push)) \
        __pragma(warning(suppress: 4100)) \
        _unused_ ## name \
        __pragma(warning(pop))
#else
#  define ALIF_UNUSED(name) _unused_ ## name
#endif


#define ALIFSUB_CONTAINER_OF(_ptr, _type, _member) \
    (_type*)((char*)_ptr - offsetof(_type, _member))

#define ALIF_RVALUE(expr) ((void)0, (expr))
