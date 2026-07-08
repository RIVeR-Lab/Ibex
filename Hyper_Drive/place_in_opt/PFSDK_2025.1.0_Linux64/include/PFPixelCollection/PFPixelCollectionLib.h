
#ifndef PFPixelCollectionLib_API_H
#define PFPixelCollectionLib_API_H

#ifdef PFPIXELCOLLECTION_STATIC_DEFINE
#  define PFPixelCollectionLib_API
#  define PFPIXELCOLLECTION_NO_EXPORT
#else
#  ifndef PFPixelCollectionLib_API
#    ifdef PFPixelCollection_EXPORTS
        /* We are building this library */
#      define PFPixelCollectionLib_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define PFPixelCollectionLib_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef PFPIXELCOLLECTION_NO_EXPORT
#    define PFPIXELCOLLECTION_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef PFPIXELCOLLECTION_DEPRECATED
#  define PFPIXELCOLLECTION_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef PFPIXELCOLLECTION_DEPRECATED_EXPORT
#  define PFPIXELCOLLECTION_DEPRECATED_EXPORT PFPixelCollectionLib_API PFPIXELCOLLECTION_DEPRECATED
#endif

#ifndef PFPIXELCOLLECTION_DEPRECATED_NO_EXPORT
#  define PFPIXELCOLLECTION_DEPRECATED_NO_EXPORT PFPIXELCOLLECTION_NO_EXPORT PFPIXELCOLLECTION_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PFPIXELCOLLECTION_NO_DEPRECATED
#    define PFPIXELCOLLECTION_NO_DEPRECATED
#  endif
#endif

#endif /* PFPixelCollectionLib_API_H */
