#ifndef POINT3DUTILS_H
#define POINT3DUTILS_H

#include "Point3D.h"
#include <vector>

Point3D addPoints(const Point3D& p1, const Point3D& p2);

Point3D scalePoint(const Point3D& p, float scalar);

Point3D subtractPoints(const Point3D& p1, const Point3D& p2);

Point3D crossProduct(const Point3D& v1, const Point3D& v2);

Point3D normalize(const Point3D& v);

Point3D calculateNormal(const std::vector<std::vector<Point3D>>& surfaceGrid, int i, int j);

#endif

