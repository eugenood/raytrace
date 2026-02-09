#pragma once

struct Vector3D
{
    double x{};
    double y{};
    double z{};

    Vector3D operator-() const;
};

double length(const Vector3D& v);
double lengthSquared(const Vector3D& v);
Vector3D unit(const Vector3D& v);
bool isNearZero(const Vector3D& v);

Vector3D operator+(const Vector3D& u, const Vector3D& v);
Vector3D operator-(const Vector3D& u, const Vector3D& v);
Vector3D operator*(const Vector3D& u, const Vector3D& v);
Vector3D operator*(const Vector3D& v, double t);
Vector3D operator*(double t, const Vector3D& v);
Vector3D operator/(const Vector3D& v, double t);

double dot(const Vector3D& u, const Vector3D& v);
Vector3D cross(const Vector3D& u, const Vector3D& v);
Vector3D reflect(const Vector3D& v, const Vector3D& normal);
Vector3D refract(const Vector3D& v, const Vector3D& normal, double refractIndex);

using Point3D = Vector3D;