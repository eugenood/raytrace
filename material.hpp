#pragma once

#include "hittable.hpp"
#include "image.hpp"
#include "ray.hpp"

#include <optional>
#include <utility>

class HitRecord;

class Material
{
public:
    virtual ~Material() = default;
    virtual std::optional<std::pair<Ray, Color>> scatter(const Ray& ray, const HitRecord& hitRecord) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color& albedo);
    std::optional<std::pair<Ray, Color>> scatter(const Ray& ray, const HitRecord& hitRecord) const override;

private:
    Color m_albedo{};
};

class Metal : public Material
{
public:
    Metal(const Color& albedo, double fuzz);
    std::optional<std::pair<Ray, Color>> scatter(const Ray& ray, const HitRecord& hitRecord) const override;

private:
    Color m_albedo{};
    double m_fuzz{};
};

class Dielectric : public Material
{
public:
    Dielectric(double refractIndex);
    std::optional<std::pair<Ray, Color>> scatter(const Ray& ray, const HitRecord& hitRecord) const override;

private:
    double m_refractIndex{};
    double reflectance(double cosTheta) const;
};