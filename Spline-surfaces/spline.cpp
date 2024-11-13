#include "spline.h"

// Calculate the B-spline basis function value N_{i,p}(u)
float BSplineBasis(int i, int p, float u, const std::vector<float>& knotVector) {
    if (p == 0) {
        return (u >= knotVector[i] && u <= knotVector[i + 1]) ? 1.0f : 0.0f;
    }

    float leftTerm = 0.0f;
    if (knotVector[i + p] != knotVector[i]) {
        leftTerm = (u - knotVector[i]) / (knotVector[i + p] - knotVector[i]) * BSplineBasis(i, p - 1, u, knotVector);
    }

    float rightTerm = 0.0f;
    if (knotVector[i + p + 1] != knotVector[i + 1]) {
        rightTerm = (knotVector[i + p + 1] - u) / (knotVector[i + p + 1] - knotVector[i + 1]) * BSplineBasis(i + 1, p - 1, u, knotVector);
    }

    return leftTerm + rightTerm;
}

// Generate a uniform knot vector
std::vector<float> generateKnotVector(int numControlPoints, int degree) {
    int n = numControlPoints + degree + 1;
    std::vector<float> knotVector(n);

    for (int i = 0; i <= degree; ++i) {
        knotVector[i] = 0.0f;
    }
    for (int i = degree + 1; i < n - degree - 1; ++i) {
        knotVector[i] = static_cast<float>(i - degree) / (n - 2 * degree - 1);
    }
    for (int i = n - degree - 1; i < n; ++i) {
        knotVector[i] = 1.0f;
    }

    return knotVector;
}


// Function to generate a B-Spline surface
std::vector<std::vector<Point3D>> BSplineSurface(const std::vector<std::vector<Point3D>>& controlGrid, int uResolution, int vResolution, int uDegree, int vDegree) {
    int n = controlGrid.size();
    int m = controlGrid[0].size();

    if (uDegree >= n || vDegree >= m) {
        throw std::invalid_argument("Degree cannot be greater than or equal to the number of control points.");
    }

    std::vector<float> uKnotVector = generateKnotVector(n, uDegree);
    std::vector<float> vKnotVector = generateKnotVector(m, vDegree);

    std::vector<std::vector<Point3D>> surfaceGrid(uResolution, std::vector<Point3D>(vResolution));

    for (int uIndex = 0; uIndex < uResolution; uIndex++) {
        float u = static_cast<float>(uIndex) / (uResolution-1);
        
        for (int vIndex = 0; vIndex < vResolution; vIndex++) {
            float v = static_cast<float>(vIndex) / (vResolution-1);
 
            Point3D point(0.0f, 0.0f, 0.0f);

            // Calculate the B-spline surface point
            for (int i = 0; i < n; ++i) {
                float Ni_u = BSplineBasis(i, uDegree, u, uKnotVector);

                for (int j = 0; j < m; ++j) {
                    float Nj_v = BSplineBasis(j, vDegree, v, vKnotVector);

                    // Add contribution of control point to the surface point
                    Point3D scaledPoint = scalePoint(controlGrid[i][j], Ni_u * Nj_v);
                    point = addPoints(point, scaledPoint);
                }
            }

            surfaceGrid[uIndex][vIndex] = point;
        }
    }

    return surfaceGrid;
}