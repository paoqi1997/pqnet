#include "util.h"

int add(int x, int y)
{
    return x + y;
}

int getElement(int m[], int index)
{
    return m[index];
}

void setX(Coord *coord, int x)
{
    coord->x = x;
}

void setY(Coord *coord, int y)
{
    coord->y = y;
}
