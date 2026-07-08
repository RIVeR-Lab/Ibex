#ifndef PFRANGEMAPVIEWERLIB_H
#define PFRANGEMAPVIEWERLIB_H

#ifdef WIN32
#ifdef PFRANGEMAPVIEWER_LIBRARY
#define PFRANGEMAPVIEWER_EXPORT __declspec(dllexport)
#else
#define PFRANGEMAPVIEWER_EXPORT __declspec(dllimport)
#endif
#else
#define PFRANGEMAPVIEWER_EXPORT
#endif

#endif // PFRANGEMAPVIEWERLIB_H

