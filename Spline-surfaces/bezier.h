#ifndef BEZIER_H
#define BEZIER_H

#include "Point3DUtils.h"
#include <cmath>

int binomialCoefficient(int n, int k);

float bernstein(int i, int n, float t);

std::vector<std::vector<Point3D>> BezierSurface(const std::vector<std::vector<Point3D>>& grid, int resolution);

#endif