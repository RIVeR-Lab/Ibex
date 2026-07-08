
#ifndef PFDOUBLERATE_API_H
#define PFDOUBLERATE_API_H

#ifdef PFDOUBLERATE_STATIC_DEFINE
#  define PFDOUBLERATE_API
#  define PFDOUBLERATE_NO_EXPORT
#else
#  ifndef PFDOUBLERATE_API
#    ifdef pfDoubleRate_EXPORTS
        /* We are building this library */
#      define PFDOUBLERATE_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define PFDOUBLERATE_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef PFDOUBLERATE_NO_EXPORT
#    define PFDOUBLERATE_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef PFDOUBLERATE_DEPRECATED
#  define PFDOUBLERATE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef PFDOUBLERATE_DEPRECATED_EXPORT
#  define PFDOUBLERATE_DEPRECATED_EXPORT PFDOUBLERATE_API PFDOUBLERATE_DEPRECATED
#endif

#ifndef PFDOUBLERATE_DEPRECATED_NO_EXPORT
#  define PFDOUBLERATE_DEPRECATED_NO_EXPORT PFDOUBLERATE_NO_EXPORT PFDOUBLERATE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PFDOUBLERATE_NO_DEPRECATED
#    define PFDOUBLERATE_NO_DEPRECATED
#  endif
#endif

#endif /* PFDOUBLERATE_API_H */
