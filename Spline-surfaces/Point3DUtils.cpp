#include "Point3DUtils.h"

// Function to add two Point3D points
Point3D addPoints(const Point3D& p1, const Point3D& p2) {
    return Point3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

// Function to scale a Point3D by a scalar
Point3D scalePoint(const Point3D& p, float scalar) {
    return Point3D(p.x * scalar, p.y * scalar, p.z * scalar);
}

// Pont kivon�s, ami vektort ad k�t pont k�z�tt
Point3D subtractPoints(const Point3D& p1, const Point3D& p2) {
	return { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
}

// Keresztterm�k sz�m�t�sa k�t vektor k�z�tt
Point3D crossProduct(const Point3D& v1, const Point3D& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

// Vektor norm�l�sa (hossza legyen 1)
Point3D normalize(const Point3D& v) {
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0) return { 0.0f, 0.0f, 1.0f };  // Alap�rtelmezett norm�l, ha a hossz 0
	return { v.x / length, v.y / length, v.z / length };
}

Point3D calculateNormal(const std::vector<std::vector<Point3D>>& surfaceGrid, int i, int j) {
	// Ellen�rizz�k, hogy van-e el�g szomsz�d a norm�l kisz�m�t�s�hoz
	if (i + 1 < surfaceGrid.size() && j + 1 < surfaceGrid[i].size()) {
		// K�t szomsz�dos vektor a norm�lhoz
		Point3D vec1 = subtractPoints(surfaceGrid[i + 1][j], surfaceGrid[i][j]);
		Point3D vec2 = subtractPoints(surfaceGrid[i][j + 1], surfaceGrid[i][j]);

		// Keresztterm�k, hogy norm�lvektort kapjunk
		Point3D normal = crossProduct(vec2, vec1);

		// Norm�l�s a hossz egys�gre
		return normalize(normal);
	}
	else if (j != 0 && i + 1 < surfaceGrid.size()) {
		Point3D vec1 = subtractPoints(surfaceGrid[i + 1][j], surfaceGrid[i][j]);
		Point3D vec2 = subtractPoints(surfaceGrid[i][j - 1], surfaceGrid[i][j]);

		// Keresztterm�k, hogy norm�lvektort kapjunk
		Point3D normal = crossProduct(vec1, vec2);

		// Norm�l�s a hossz egys�gre
		return normalize(normal);
	}

	// Ha nincs el�g szomsz�d, visszaadunk egy alap�rtelmezett norm�lt
	return { 0.0f, 0.0f, 1.0f };
}