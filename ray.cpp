#include "ray.hpp"
#include "vector.hpp"

Ray::Ray(const Point3D& origin, const Vector3D& direction)
    : m_origin{origin}
    , m_direction{unit(direction)}
{
    // pass
}

const Point3D& Ray::origin() const
{
    return m_origin;
}

const Vector3D& Ray::direction() const
{
    return m_direction;
}

Vector3D Ray::operator()(double t) const
{
    return m_origin + m_direction * t;
}