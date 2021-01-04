#ifndef UTIL_H
#define UTIL_H

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

struct Coord
{
    int x;
    int y;
};

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORT int add(int x, int y);

DLL_EXPORT int getElement(int m[], int index);

DLL_EXPORT void setX(Coord *coord, int x);
DLL_EXPORT void setY(Coord *coord, int y);

#ifdef __cplusplus
}
#endif

#endif // UTIL_H
