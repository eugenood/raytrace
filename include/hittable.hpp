#pragma once

#include "interval.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <memory>
#include <optional>
#include <vector>

class Material;

class HitRecord
{
public:
    HitRecord(const Ray& ray, const Point3D& position, const Vector3D& outwardNormal, std::shared_ptr<Material> material, double distance);
    const Point3D& position() const;
    const Vector3D& normal() const;
    const Material* material() const;
    double distance() const;
    bool isFrontFacing() const;

private:
    Point3D m_position{};
    Vector3D m_normal{};
    std::shared_ptr<Material> m_material{};
    double m_distance{};
    bool m_isFrontFacing{};
};

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual std::optional<HitRecord> hit(const Ray& ray, const Interval& distanceLimit) const = 0;
};

class Sphere : public Hittable
{
public:
    Sphere(const Point3D& center, double radius, std::shared_ptr<Material> material);
    std::optional<HitRecord> hit(const Ray& ray, const Interval& distanceLimit) const override;

private:
    Point3D m_center{};
    double m_radius{};
    std::shared_ptr<Material> m_material{};
};

class World : public Hittable
{
public:
    std::optional<HitRecord> hit(const Ray& ray, const Interval& distanceLimit) const override;
    void add(std::shared_ptr<Hittable> hittable);

private:
    std::vector<std::shared_ptr<Hittable>> m_hittables{};
};