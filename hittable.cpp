#include "hittable.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <cmath>
#include <memory>
#include <optional>

HitRecord::HitRecord(const Ray& ray, const Point3D& position, const Vector3D& outwardNormal, std::shared_ptr<Material> material, double distance)
    : m_position{position}
    , m_material{material}
    , m_distance{distance}
{
    m_isFrontFacing = dot(ray.direction(), outwardNormal) < 0.0;
    m_normal = m_isFrontFacing ? outwardNormal : -outwardNormal;
}

const Point3D& HitRecord::position() const
{
    return m_position;
}

const Vector3D& HitRecord::normal() const
{
    return m_normal;
}

const Material* HitRecord::material() const
{
    return m_material.get();
}

double HitRecord::distance() const
{
    return m_distance;
}

bool HitRecord::isFrontFacing() const
{
    return m_isFrontFacing;
}

Sphere::Sphere(const Point3D& center, double radius, std::shared_ptr<Material> material)
    : m_center{center}
    , m_radius{radius}
    , m_material{material}
{
    // pass
}

std::optional<HitRecord> Sphere::hit(const Ray& ray, const Interval& distanceLimit) const
{
    Vector3D rayToSphere{m_center - ray.origin()};

    double a{lengthSquared(ray.direction())};
    double h{dot(ray.direction(), rayToSphere)};
    double c{lengthSquared(rayToSphere) - m_radius * m_radius};

    double discriminant{h * h - a * c};
    if (discriminant < 0)
        return std::nullopt;

    double discriminantSqrt{std::sqrt(discriminant)};
    double distance{(h - discriminantSqrt) / a};
    if (!surrounds(distanceLimit, distance))
    {
        distance = (h + discriminantSqrt) / a;
        if (!surrounds(distanceLimit, distance))
            return std::nullopt;
    }

    Point3D hitPosition{ray(distance)};
    return HitRecord{ray, hitPosition, (hitPosition - m_center) / m_radius, m_material, distance};
}

std::optional<HitRecord> World::hit(const Ray& ray, const Interval& distanceLimit) const
{
    std::optional<HitRecord> hitRecord{};
    Interval tightestDistanceLimit{distanceLimit};

    for (const auto& hittable : m_hittables)
    {
        std::optional<HitRecord> currHitRecord{hittable->hit(ray, tightestDistanceLimit)};
        if (currHitRecord)
        {
            hitRecord = std::move(currHitRecord);
            tightestDistanceLimit.max = hitRecord->distance();
        }
    }

    return hitRecord;
}

void World::add(std::shared_ptr<Hittable> hittable)
{
    m_hittables.push_back(hittable);
}