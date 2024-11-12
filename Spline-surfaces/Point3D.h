#ifndef POINT3D_H
#define POINT3D_H

// Definition of the Point3D struct
struct Point3D {
    float x;
    float y;
    float z;

    Point3D() : x(0.0f), y(0.0f), z(0.0f) {} // Default constructor
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {} // Parameterized constructor
};

#endif // POINT3D_H
