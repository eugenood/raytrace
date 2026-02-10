#include "camera.hpp"
#include "hittable.hpp"
#include "image.hpp"
#include "material.hpp"
#include "util.hpp"
#include "vector.hpp"

#include <iostream>
#include <memory>
#include <string>

World generateDefaultWorld();
World generateCubeWorld();

int main()
{
    double aspectRatio{16.0 / 9.0};
    int imageWidth{800};
    int imageHeight{static_cast<int>(imageWidth / aspectRatio)};

    Point3D cameraOrigin{18.0, 4.0, 18.0};
    Point3D lookAt{0.8, 0.0, 0.8};
    double verticalFov{0.35};
    double defocusAngle{0.01};
    double focusDistance{length(cameraOrigin - lookAt)};
    Camera camera{cameraOrigin, lookAt, verticalFov, defocusAngle, focusDistance};

    int samplesPerPixel{200};
    int maxBounce{30};

    World world{generateCubeWorld()};
    Image image{camera.render(world, imageWidth, imageHeight, samplesPerPixel, maxBounce)};

    const std::string filename{"output.bmp"};
    if (saveImageAsBitmap(image, filename))
        std::cout << "File " << filename << " generated.\n";

    return 0;
}

World generateDefaultWorld()
{
    auto materialGround{std::make_shared<Lambertian>(Color{0.5, 0.5, 0.5})};

    World world{};
    world.add(std::make_shared<Sphere>(Point3D{0.0, -1000.0, 0.0}, 1000.0, materialGround));

    for (int i{-11}; i < 11; ++i)
    {
        for (int j{-11}; j < 11; ++j)
        {
            double randDouble{Random::sampleDouble()};
            Point3D center{i + 0.9 * Random::sampleDouble(), 0.2, j + 0.9 * Random::sampleDouble()};

            if (length(center - Point3D(4.0, 0.2, 0.0)) > 0.9)
            {
                if (randDouble < 0.8)
                {
                    Color albedo{Random::sampleColor() * Random::sampleColor()};
                    auto material{std::make_shared<Lambertian>(albedo)};
                    world.add(std::make_shared<Sphere>(center, 0.2, material));
                }

                else if (randDouble < 0.95)
                {
                    Color albedo{Random::sampleColor()};
                    double fuzz{Random::sampleDouble() * 0.5};
                    auto material{std::make_shared<Metal>(albedo, fuzz)};
                    world.add(std::make_shared<Sphere>(center, 0.2, material));
                }

                else
                {
                    auto material{std::make_shared<Dielectric>(1.5)};
                    world.add(std::make_shared<Sphere>(center, 0.2, material));
                }
            }
        }
    }

    auto materialLeft{std::make_shared<Dielectric>(1.50)};
    auto materialCenter{std::make_shared<Lambertian>(Color{0.4, 0.2, 0.1})};
    auto materialRight{std::make_shared<Metal>(Color{0.7, 0.6, 0.5}, 0.05)};

    world.add(std::make_shared<Sphere>(Point3D{0.0, 1.0, 0.0}, 1.0, materialLeft));
    world.add(std::make_shared<Sphere>(Point3D{-4.0, 1.0, 0.0}, 1.0, materialCenter));
    world.add(std::make_shared<Sphere>(Point3D{4.0, 1.0, 0.0}, 1.0, materialRight));

    return world;
}

World generateCubeWorld()
{
    const int size{2};
    const double radius{0.499};

    World world{};

    auto materialGround{std::make_shared<Metal>(Color{0.2, 0.2, 0.2}, 0.1)};
    world.add(std::make_shared<Sphere>(Point3D{0.0, -1000.0 - size - radius, 0.0}, 1000.0, materialGround));

    auto materialCopper{std::make_shared<Metal>(Color{0.8, 0.38, 0.15}, 0.01)};
    auto materialBrushedCopper{std::make_shared<Metal>(Color{0.8, 0.38, 0.15}, 0.25)};
    auto materialGlass{std::make_shared<Dielectric>(1.5)};
    auto materialMatteWhite{std::make_shared<Lambertian>(Color{0.9, 0.9, 0.85})};

    for (int i{-size}; i <= size; ++i)
    {
        for (int j{-size}; j <= size; ++j)
        {
            for (int k{-size}; k <= size; ++k)
            {
                Point3D center{static_cast<double>(i), static_cast<double>(j), static_cast<double>(k)};
                double randDouble{Random::sampleDouble()};
                if (randDouble < 0.2)
                    world.add(std::make_shared<Sphere>(center, radius, materialCopper));
                else if (randDouble < 0.4)
                    world.add(std::make_shared<Sphere>(center, radius, materialBrushedCopper));
                else if (randDouble < 0.7)
                    world.add(std::make_shared<Sphere>(center, radius, materialMatteWhite));
                else
                    world.add(std::make_shared<Sphere>(center, radius, materialGlass));
            }
        }
    }

    return world;
}