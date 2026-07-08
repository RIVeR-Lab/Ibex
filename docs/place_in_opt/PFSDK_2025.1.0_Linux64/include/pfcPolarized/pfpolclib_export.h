
#ifndef PFPOLCLIB_EXPORT_H
#define PFPOLCLIB_EXPORT_H

#ifdef PFPOLCLIB_NODLL
#  define PFPOLCLIB_EXPORT
#  define PFPOLCLIB_NO_EXPORT
#else
#  ifndef PFPOLCLIB_EXPORT
#    ifdef PFPolarizedLib_EXPORTS
        /* We are building this library */
#      define PFPOLCLIB_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define PFPOLCLIB_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef PFPOLCLIB_NO_EXPORT
#    define PFPOLCLIB_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef PFPOLCLIB_DEPRECATED
#  define PFPOLCLIB_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef PFPOLCLIB_DEPRECATED_EXPORT
#  define PFPOLCLIB_DEPRECATED_EXPORT PFPOLCLIB_EXPORT PFPOLCLIB_DEPRECATED
#endif

#ifndef PFPOLCLIB_DEPRECATED_NO_EXPORT
#  define PFPOLCLIB_DEPRECATED_NO_EXPORT PFPOLCLIB_NO_EXPORT PFPOLCLIB_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PFPOLCLIB_NO_DEPRECATED
#    define PFPOLCLIB_NO_DEPRECATED
#  endif
#endif

#endif /* PFPOLCLIB_EXPORT_H */
