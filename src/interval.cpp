#include "interval.hpp"

double size(const Interval& interval)
{
    return interval.max - interval.min;
}

bool contains(const Interval& interval, double value)
{
    return (value >= interval.min) && (value <= interval.max);
}

bool surrounds(const Interval& interval, double value)
{
    return (value > interval.min) && (value < interval.max);
}

double clamp(const Interval& interval, double value)
{
    if (value > interval.max)
        return interval.max;
    if (value < interval.min)
        return interval.min;
    return value;
}