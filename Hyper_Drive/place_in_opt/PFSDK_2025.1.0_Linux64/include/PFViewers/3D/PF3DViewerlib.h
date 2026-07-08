#ifndef PF3DVIEWERLIB_H
#define PF3DVIEWERLIB_H

#ifdef WIN32
#ifdef PF3DVIEWER_LIBRARY
#define PF3DVIEWER_EXPORT __declspec(dllexport)
#else
#define PF3DVIEWER_EXPORT __declspec(dllimport)
#endif
#elif defined(__linux__) || defined(__APPLE__)
#ifdef PF3DVIEWER_LIBRARY
#define PF3DVIEWER_EXPORT __attribute__((visibility("default")))
#else
#define PF3DVIEWER_EXPORT
#endif
#else
#define PF3DVIEWER_EXPORT
#endif

#endif // PFRANGEMAPVIEWERLIB_H

