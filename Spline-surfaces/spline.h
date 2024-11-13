#ifndef BSPLINESURFACE_H
#define BSPLINESURFACE_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include "Point3DUtils.h"

float BSplineBasis(int i, int p, float u, const std::vector<float>& knotVector);

std::vector<float> generateKnotVector(int numControlPoints, int degree);

std::vector<std::vector<Point3D>> BSplineSurface(const std::vector<std::vector<Point3D>>& controlGrid, int uResolution, int vResolution, int uDegree, int vDegree);


#endif // BSPLINESURFACE_H
