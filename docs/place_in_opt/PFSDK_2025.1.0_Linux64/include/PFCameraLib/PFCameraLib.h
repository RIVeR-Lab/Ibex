
#ifndef PFCAMERALIB_API_H
#define PFCAMERALIB_API_H

#ifdef PFCAMERALIB_STATIC_DEFINE
#  define PFCAMERALIB_API
#  define PFCAMERALIB_NO_EXPORT
#else
#  ifndef PFCAMERALIB_API
#    ifdef PFCameraLib_EXPORTS
        /* We are building this library */
#      define PFCAMERALIB_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define PFCAMERALIB_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef PFCAMERALIB_NO_EXPORT
#    define PFCAMERALIB_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef PFCAMERALIB_DEPRECATED
#  define PFCAMERALIB_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef PFCAMERALIB_DEPRECATED_EXPORT
#  define PFCAMERALIB_DEPRECATED_EXPORT PFCAMERALIB_API PFCAMERALIB_DEPRECATED
#endif

#ifndef PFCAMERALIB_DEPRECATED_NO_EXPORT
#  define PFCAMERALIB_DEPRECATED_NO_EXPORT PFCAMERALIB_NO_EXPORT PFCAMERALIB_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PFCAMERALIB_NO_DEPRECATED
#    define PFCAMERALIB_NO_DEPRECATED
#  endif
#endif

#endif /* PFCAMERALIB_API_H */
