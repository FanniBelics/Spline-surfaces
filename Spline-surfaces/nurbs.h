#ifndef NURBS_H
#define NURBS_H

#include <cmath>
#include <vector>
#include "Point3DUtils.h"
#include "spline.h"

std::vector<std::vector<Point3D>> NURBSSurface(const std::vector<std::vector<Point3D>>& controlGrid, int uResolution, int vResolution, int uDegree, int vDegree, bool isClamped);

#endif

