#include "material.hpp"
#include "image.hpp"
#include "util.hpp"
#include "vector.hpp"

#include <cmath>
#include <utility>

Lambertian::Lambertian(const Color& albedo)
    : m_albedo{albedo}
{
    // pass
}

std::optional<std::pair<Ray, Color>> Lambertian::scatter(const Ray&, const HitRecord& hitRecord) const
{
    Vector3D scatteredDirection{hitRecord.normal() + Random::sampleUnitVector()};
    if (isNearZero(scatteredDirection))
        scatteredDirection = hitRecord.normal();

    Ray scatteredRay{hitRecord.position(), scatteredDirection};
    return std::make_pair(scatteredRay, m_albedo);
}

Metal::Metal(const Color& albedo, double fuzz)
    : m_albedo{albedo}
    , m_fuzz{fuzz}
{
    // pass
}

std::optional<std::pair<Ray, Color>> Metal::scatter(const Ray& ray, const HitRecord& hitRecord) const
{
    Vector3D scatteredDirection{reflect(ray.direction(), hitRecord.normal()) + m_fuzz * Random::sampleUnitVector()};
    Ray scatteredRay{hitRecord.position(), scatteredDirection};
    return std::make_pair(scatteredRay, m_albedo);
}

Dielectric::Dielectric(double refractIndex)
    : m_refractIndex{refractIndex}
{
    // pass
}

std::optional<std::pair<Ray, Color>> Dielectric::scatter(const Ray& ray, const HitRecord& hitRecord) const
{
    double refractIndex{hitRecord.isFrontFacing() ? (1.0 / m_refractIndex) : m_refractIndex};
    double cosTheta{std::fmin(dot(-ray.direction(), hitRecord.normal()), 1.0)};
    double sinTheta{std::sqrt(1.0 - cosTheta * cosTheta)};

    Vector3D scatteredDirection{};
    if (refractIndex * sinTheta > 1.0 || reflectance(cosTheta) > Random::sampleDouble())
        scatteredDirection = reflect(ray.direction(), hitRecord.normal());
    else
        scatteredDirection = refract(ray.direction(), hitRecord.normal(), refractIndex);

    Ray scatteredRay{hitRecord.position(), scatteredDirection};
    return std::make_pair(scatteredRay, Color{1.0, 1.0, 1.0});
}

double Dielectric::reflectance(double cosTheta) const
{
    double r0{(1.0 - m_refractIndex) / (1.0 + m_refractIndex)};
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow((1 - cosTheta), 5); 
}