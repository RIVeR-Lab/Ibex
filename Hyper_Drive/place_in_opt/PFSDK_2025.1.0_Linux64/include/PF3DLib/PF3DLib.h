
#ifndef PF3DLIB_API_H
#define PF3DLIB_API_H

#ifdef PF3DLIB_STATIC_DEFINE
#  define PF3DLIB_API
#  define PF3DLIB_NO_EXPORT
#else
#  ifndef PF3DLIB_API
#    ifdef PF3DLib_EXPORTS
        /* We are building this library */
#      define PF3DLIB_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define PF3DLIB_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef PF3DLIB_NO_EXPORT
#    define PF3DLIB_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef PF3DLIB_DEPRECATED
#  define PF3DLIB_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef PF3DLIB_DEPRECATED_EXPORT
#  define PF3DLIB_DEPRECATED_EXPORT PF3DLIB_API PF3DLIB_DEPRECATED
#endif

#ifndef PF3DLIB_DEPRECATED_NO_EXPORT
#  define PF3DLIB_DEPRECATED_NO_EXPORT PF3DLIB_NO_EXPORT PF3DLIB_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PF3DLIB_NO_DEPRECATED
#    define PF3DLIB_NO_DEPRECATED
#  endif
#endif

#endif /* PF3DLIB_API_H */
