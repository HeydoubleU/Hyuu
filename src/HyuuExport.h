
#ifndef HYUU_EXPORT_H
#define HYUU_EXPORT_H

#if defined(_WIN32)
#define HYUU_EXPORT __declspec(dllexport)
#define HYUU_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#define HYUU_EXPORT __attribute__((visibility("default")))
#define HYUU_IMPORT __attribute__((visibility("default")))
#else
#error Unsupported platform.
#endif

#if defined(HYUU_BUILD_NODEDEF_DLL)
#define HYUU_DECL HYUU_EXPORT
#else
#define HYUU_DECL HYUU_IMPORT
#endif

#endif
