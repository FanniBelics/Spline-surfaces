#include "bezier.h"

// Helper function to calculate binomial coefficients
int binomialCoefficient(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    int result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

// Helper function to calculate the Bernstein polynomial
float bernstein(int i, int n, float t) {
    return binomialCoefficient(n, i) * std::pow(t, i) * std::pow(1 - t, n - i);
}

// Function to generate a Bezier surface grid based on the control points grid
std::vector<std::vector<Point3D>> BezierSurface(const std::vector<std::vector<Point3D>>& grid, int resolution) {
    int n = grid.size() - 1;      // Degree in u-direction
    int m = grid[0].size() - 1;   // Degree in v-direction

    std::vector<std::vector<Point3D>> surfaceGrid(resolution, std::vector<Point3D>(resolution));

    for (int uIndex = 0; uIndex < resolution; ++uIndex) {
        float u = static_cast<float>(uIndex) / (resolution - 1);  // Parametrize u in [0, 1]

        for (int vIndex = 0; vIndex < resolution; ++vIndex) {
            float v = static_cast<float>(vIndex) / (resolution - 1);  // Parametrize v in [0, 1]

            Point3D point(0.0f, 0.0f, 0.0f);  // Initialize the point at the surface

            // Calculate the surface point using the Bernstein polynomials
            for (int i = 0; i <= n; ++i) {
                float bernsteinU = bernstein(i, n, u);

                for (int j = 0; j <= m; ++j) {
                    float bernsteinV = bernstein(j, m, v);

                    // Add the contribution of the control point to the surface point
                    Point3D scaledPoint = scalePoint(grid[i][j], bernsteinU * bernsteinV);
                    point = addPoints(point, scaledPoint);
                }
            }

            surfaceGrid[uIndex][vIndex] = point;
        }
    }

    return surfaceGrid;
}
