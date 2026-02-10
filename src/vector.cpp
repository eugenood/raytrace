#include "vector.hpp"

#include <cmath>

Vector3D Vector3D::operator-() const
{
    return Vector3D{-x, -y, -z};
}

double length(const Vector3D& v)
{
    return std::sqrt(lengthSquared(v));
}

double lengthSquared(const Vector3D& v)
{
    return dot(v, v);
}

Vector3D unit(const Vector3D& v)
{
    return v / length(v);
}

bool isNearZero(const Vector3D& v)
{
    static const double eps{1e-8};
    return (v.x < eps) && (v.y < eps) && (v.z < eps);
}

Vector3D operator+(const Vector3D& u, const Vector3D& v)
{
    return Vector3D{u.x + v.x, u.y + v.y, u.z + v.z};
}

Vector3D operator-(const Vector3D& u, const Vector3D& v)
{
    return Vector3D{u.x - v.x, u.y - v.y, u.z - v.z};
}

Vector3D operator*(const Vector3D& u, const Vector3D& v)
{
    return Vector3D{u.x * v.x, u.y * v.y, u.z * v.z};
}

Vector3D operator*(const Vector3D& v, double t)
{
    return Vector3D{v.x * t, v.y * t, v.z * t};
}

Vector3D operator*(double t, const Vector3D& v)
{
    return Vector3D{t * v.x, t * v.y, t * v.z};
}

Vector3D operator/(const Vector3D& v, double t)
{
    return Vector3D{v.x / t, v.y / t, v.z / t};
}

double dot(const Vector3D& u, const Vector3D& v)
{
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

Vector3D cross(const Vector3D& u, const Vector3D& v)
{
    return Vector3D{
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    };
}

Vector3D reflect(const Vector3D& v, const Vector3D& normal)
{
    return v - 2 * dot(v, normal) * normal;
}

Vector3D refract(const Vector3D& v, const Vector3D& normal, double refractIndex)
{
    double cosTheta{std::fmin(dot(-v, normal), 1.0)};
    Vector3D outPerp{refractIndex * (v + cosTheta * normal)};
    Vector3D outPara{-std::sqrt(std::fabs(1.0 - lengthSquared(outPerp))) * normal};
    return outPerp + outPara;
}