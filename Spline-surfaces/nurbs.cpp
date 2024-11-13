#include "nurbs.h"

std::vector<std::vector<Point3D>> NURBSSurface(const std::vector<std::vector<Point3D>>& controlGrid, int uResolution, int vResolution, int uDegree, int vDegree) {
    int n = controlGrid.size();
    int m = controlGrid[0].size();

    if (uDegree >= n || vDegree >= m) {
        throw std::invalid_argument("Degree cannot be greater than or equal to the number of control points.");
    }

    std::vector<float> uKnotVector = generateKnotVector(n, uDegree);
    std::vector<float> vKnotVector = generateKnotVector(m, vDegree);

    std::vector<std::vector<Point3D>> surfaceGrid(uResolution, std::vector<Point3D>(vResolution));

    for (int uIndex = 0; uIndex < uResolution; ++uIndex) {
        float u = uKnotVector[uDegree] + (static_cast<float>(uIndex) / (uResolution - 1)) * (uKnotVector[n] - uKnotVector[uDegree]);

        for (int vIndex = 0; vIndex < vResolution; ++vIndex) {
            float v = vKnotVector[vDegree] + (static_cast<float>(vIndex) / (vResolution - 1)) * (vKnotVector[m] - vKnotVector[vDegree]);

            Point3D numerator(0.0f, 0.0f, 0.0f, 0.0f); // Súlyozott összeg
            float denominator = 0.0f; // Súlyok összegzése

            // Számoljuk ki a NURBS felület pontját
            for (int i = 0; i < n; ++i) {
                float Ni_u = BSplineBasis(i, uDegree, u, uKnotVector);

                for (int j = 0; j < m; ++j) {
                    float Nj_v = BSplineBasis(j, vDegree, v, vKnotVector);
                    float weight = controlGrid[i][j].w;

                    // Súlyozott pontszámítás a kontrollpontokkal
                    numerator.x += controlGrid[i][j].x * Ni_u * Nj_v * weight;
                    numerator.y += controlGrid[i][j].y * Ni_u * Nj_v * weight;
                    numerator.z += controlGrid[i][j].z * Ni_u * Nj_v * weight;
                    numerator.w += weight * Ni_u * Nj_v; // Súly hozzáadása

                    // Súlyok összegzése
                    denominator += weight * Ni_u * Nj_v;
                }
            }

            // Normalizálás a NURBS pont számításához
            if (denominator != 0.0f) {
                surfaceGrid[uIndex][vIndex] = Point3D(numerator.x / denominator, numerator.y / denominator, numerator.z / denominator);
            }
            else {
                surfaceGrid[uIndex][vIndex] = Point3D(); // Ha a nevezõ 0, akkor alapértelmezett pont
            }
        }
    }

    return surfaceGrid;
}