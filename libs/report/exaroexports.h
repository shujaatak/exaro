#ifndef EXARO_EXPORTS_H
#define EXARO_EXPORTS_H

#ifdef WIN32
#ifdef LIB_EXPORTS
#define EXARO_EXPORTS __declspec(dllexport)
#else
#define EXARO_EXPORTS __declspec(dllimport)
#endif
#else
#define EXARO_EXPORTS
#endif

#endif
