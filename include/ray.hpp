#pragma once

#include "vector.hpp"

class Ray
{
public:
    Ray(const Point3D& origin, const Vector3D& direction);
    const Point3D& origin() const;
    const Vector3D& direction() const;
    Vector3D operator()(double t) const;

private:
    Point3D m_origin{};
    Vector3D m_direction{};
};