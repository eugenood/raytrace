#include "camera.hpp"
#include "hittable.hpp"
#include "image.hpp"
#include "util.hpp"
#include "vector.hpp"

#include <atomic>
#include <cmath>
#include <cstddef>
#include <iostream>

Camera::Camera(const Point3D& cameraOrigin, const Point3D& lookAt, double verticalFov, double defocusAngle, double focusDistance)
    : m_cameraOrigin{cameraOrigin}
    , m_lookAt{lookAt}
    , m_verticalFov{verticalFov}
    , m_defocusAngle{defocusAngle}
    , m_focusDistance{focusDistance}
{
    // pass
}

Image Camera::render(const World& world, int imageWidth, int imageHeight, int samplesPerPixel, int maxBounce)
{
    const double halfHeight{std::tan(m_verticalFov / 2)};
    const double viewportHeight{2 * halfHeight * m_focusDistance};
    const double viewportWidth{viewportHeight * (static_cast<double>(imageWidth) / imageHeight)};

    const Vector3D cameraW{unit(m_cameraOrigin - m_lookAt)};
    const Vector3D cameraU{unit(cross(Vector3D{0.0, 1.0, 0.0}, cameraW))};
    const Vector3D cameraV{cross(cameraW, cameraU)};

    const Vector3D viewportU{cameraU * viewportWidth};
    const Vector3D viewportV{cameraV * viewportHeight};
    const Point3D viewportOrigin{m_cameraOrigin - (m_focusDistance * cameraW) - 0.5 * (viewportU + viewportV)};

    const Vector3D pixelDeltaU{viewportU / imageWidth};
    const Vector3D pixelDeltaV{viewportV / imageHeight};
    const Point3D pixelFirstOrigin{viewportOrigin + 0.5 * (pixelDeltaU + pixelDeltaV)};

    const double defocusRadius{m_focusDistance * std::tan(m_defocusAngle / 2.0)};
    const Vector3D defocusDiskU{cameraU * defocusRadius};
    const Vector3D defocusDiskV{cameraV * defocusRadius};

    Image image{imageWidth, imageHeight};
    std::atomic<int> linesFinished{0};

    #pragma omp parallel for schedule(dynamic, 1)
    for (int y = 0; y < imageHeight; ++y)
    {
        for (int x{0}; x < imageWidth; ++x)
        {
            Color color{};

            for (int i{0}; i < samplesPerPixel; ++i)
            {
                Vector3D offset{Random::sampleSquare()};
                Vector3D rayOrigin = (m_defocusAngle <= 0) ? m_cameraOrigin : sampleDefocusDisk(defocusDiskU, defocusDiskV);
                Vector3D rayDirection{pixelFirstOrigin + (pixelDeltaU * (x + offset.x)) + (pixelDeltaV * (y + offset.y)) - rayOrigin};
                Ray ray{rayOrigin, rayDirection};
                color += getRayColor(ray, world, maxBounce);
            }

            image(x, y) = color / samplesPerPixel;
        }

        ++linesFinished;
        std::clog << "\rScanlines remaining: " << (imageHeight - linesFinished) << "   " << std::flush;
    }

    std::clog << "\n";
    return image;
}

Color Camera::getRayColor(const Ray& ray, const World& world, int bounceLeft) const
{
    if (bounceLeft <= 0)
        return Color{0.0, 0.0, 0.0};

    std::optional<HitRecord> hitRecord{world.hit(ray, Interval{0.001, Constant::infinity})};

    if (hitRecord)
    {
        auto reflection{hitRecord->material()->scatter(ray, *hitRecord)};

        if (reflection)
        {
            auto [reflectedRay, attenuation] = *reflection;
            return attenuation * getRayColor(reflectedRay, world, bounceLeft - 1);
        }
    }

    double lerp{0.5 * (ray.direction().y + 1.0)};
    return (1.0 - lerp) * Color{1.0, 1.0, 1.0} + lerp * Color{0.5, 0.7, 1.0};
}

Point3D Camera::sampleDefocusDisk(const Vector3D& defocusDiskU, const Vector3D& defocusDiskV) const
{
    Point3D sample{Random::sampleUnitDisk()};
    return m_cameraOrigin + (sample.x * defocusDiskU) + (sample.y * defocusDiskV);
}