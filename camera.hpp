#pragma once

#include "hittable.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "vector.hpp"

class Camera
{
public:
    Camera(const Point3D& cameraOrigin, const Point3D& lookAt, double verticalFov, double defocusAngle, double focusDistance);
    Image render(const World& world, int imageWidth, int imageHeight, int samplesPerPixel, int maxBounce);
    Image render(const World& world, int imageWidth, int imageHeight, int samplesPerPixel, int maxBounce, int numThreads);

private:
    Point3D m_cameraOrigin{};
    Point3D m_lookAt{};
    double m_verticalFov{};
    double m_defocusAngle{};
    double m_focusDistance{};
    Color getRayColor(const Ray& ray, const World& world, int bounceLeft) const;
    Point3D sampleDefocusDisk(const Vector3D& defocusDiskU, const Vector3D& defocusDiskV) const;
};