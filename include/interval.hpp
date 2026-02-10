#pragma once

struct Interval
{
    double min{};
    double max{};
};

double size(const Interval& interval);
bool contains(const Interval& interval, double value);
bool surrounds(const Interval& interval, double value);
double clamp(const Interval& interval, double value);