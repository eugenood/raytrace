#include "util.hpp"
#include "image.hpp"
#include "vector.hpp"

#include <cmath>
#include <random>

double Random::sampleDouble()
{
    static thread_local std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{0.0, 1.0};
    return distribution(generator);
}

double Random::sampleDouble(double min, double max)
{
    return sampleDouble() * (max - min) + min;
}

bool Random::sampleBool(double prob)
{
    return sampleDouble() <= prob;
}

Vector3D Random::sampleVector()
{
    return Vector3D{sampleDouble(), sampleDouble(), sampleDouble()};
}

Vector3D Random::sampleVector(double min, double max)
{
    return Vector3D{sampleDouble(min, max), sampleDouble(min, max), sampleDouble(min, max)};
}

Vector3D Random::sampleUnitVector()
{
    while (true)
    {
        Vector3D candidate{sampleVector(-1.0, 1.0)};
        double candidateLengthSquared{lengthSquared(candidate)};
        if (candidateLengthSquared <= 1.0 && candidateLengthSquared > 1e-160)
            return candidate / std::sqrt(candidateLengthSquared);
    }
}

Vector3D Random::sampleOnHemisphere(const Vector3D& normal)
{
    Vector3D unitVector{sampleUnitVector()};
    if (dot(unitVector, normal) > 0.0)
        return unitVector;
    return -unitVector;
}

Vector3D Random::sampleSquare()
{
    return Vector3D{sampleDouble() - 0.5, sampleDouble() - 0.5, 0.0};
}


Vector3D Random::sampleUnitDisk()
{
    while (true)
    {
        Vector3D candidate{sampleDouble(-1.0, 1.0), sampleDouble(-1.0, 1.0), 0.0};
        if (lengthSquared(candidate) < 1.0)
            return candidate;
    }
}

Color Random::sampleColor()
{
    return Color{sampleDouble(), sampleDouble(), sampleDouble()};
}