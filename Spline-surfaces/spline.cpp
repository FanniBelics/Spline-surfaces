#include "spline.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

// Helper function to calculate B-Spline basis functions
float calculateBSplineBasis(int i, int degree, float t, const std::vector<float>& knots) {
    if (degree == 0) {
        return (knots[i] <= t && t < knots[i + 1]) ? 1.0f : 0.0f;
    }

    float denom1 = knots[i + degree] - knots[i];
    float denom2 = knots[i + degree + 1] - knots[i + 1];

    float term1 = (denom1 == 0) ? 0 : ((t - knots[i]) / denom1) * calculateBSplineBasis(i, degree - 1, t, knots);
    float term2 = (denom2 == 0) ? 0 : ((knots[i + degree + 1] - t) / denom2) * calculateBSplineBasis(i + 1, degree - 1, t, knots);

    return term1 + term2;
}

// Function to draw the B-Spline surface
void drawBSplineSurface(const std::vector<std::vector<Point3D>>& controlPoints, int resolution) {
    int n = controlPoints.size();     // Number of rows of control points
    int m = controlPoints[0].size(); // Number of columns of control points
    int degree = 3;                  // Degree of the B-Spline

    // Generate uniform knot vectors
    std::vector<float> knotU(n + degree + 1);
    std::vector<float> knotV(m + degree + 1);
    for (int i = 0; i <= n + degree; ++i) {
        knotU[i] = static_cast<float>(i) / (n + degree);
    }
    for (int i = 0; i <= m + degree; ++i) {
        knotV[i] = static_cast<float>(i) / (m + degree);
    }

    // Iterate over the parametric space of the surface
    glColor3f(0.0f, 0.0f, 1.0f); // Set color to blue
    glBegin(GL_POINTS);
    for (int ru = 0; ru <= resolution; ++ru) {
        float u = static_cast<float>(ru) / resolution;

        for (int rv = 0; rv <= resolution; ++rv) {
            float v = static_cast<float>(rv) / resolution;

            // Calculate the point on the surface
            Point3D point = { 0.0f, 0.0f, 0.0f };

            for (int i = 0; i < n; ++i) {
                float bu = calculateBSplineBasis(i, degree, u, knotU);

                for (int j = 0; j < m; ++j) {
                    float bv = calculateBSplineBasis(j, degree, v, knotV);

                    point.x += bu * bv * controlPoints[i][j].x;
                    point.y += bu * bv * controlPoints[i][j].y;
                    point.z += bu * bv * controlPoints[i][j].z;
                }
            }

            // Draw the point
            glVertex3f(point.x, point.y, point.z);
        }
    }
    glEnd();
}
