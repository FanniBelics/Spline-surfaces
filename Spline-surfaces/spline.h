#ifndef BSPLINESURFACE_H
#define BSPLINESURFACE_H

#include <vector>
#include "Point3D.h"

// Function to draw a B-Spline surface
void drawBSplineSurface(const std::vector<std::vector<Point3D>>& controlPoints, int resolution);

#endif // BSPLINESURFACE_H
