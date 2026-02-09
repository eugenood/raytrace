#pragma once

#include "image.hpp"
#include "vector.hpp"

#include <limits>

namespace Constant
{
    const double infinity{std::numeric_limits<double>::infinity()};
    const double pi{3.1415926535897932385};
}

namespace Random
{
    double sampleDouble();
    double sampleDouble(double min, double max);
    bool sampleBool(double prob);
    Vector3D sampleVector();
    Vector3D sampleVector(double min, double max);
    Vector3D sampleUnitVector();
    Vector3D sampleOnHemisphere(const Vector3D& normal);
    Vector3D sampleSquare();
    Vector3D sampleUnitDisk();
    Color sampleColor();
}