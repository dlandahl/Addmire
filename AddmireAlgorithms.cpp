
#include <random>
#include <iostream>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

namespace add {

float TrackedValue::get_value()
{
    float ret = value_delta;
    value_delta = quality;
    return ret;
}

void TrackedValue::set_value(float new_value)
{
    if (quality == additive)
    {
        value_delta = (new_value - current_value);
        current_value += value_delta;
    }
    else if (quality == multiplicative)
    {
        value_delta = (new_value / current_value);
        current_value *= value_delta;
    }
}

namespace AdditiveProcesses {
void Repitch::proc()
{
    float change = value.get_value();

    for (int n = 0; n < Cluster::partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = target_cluster->partials[n];
        if (frequency >= var::get_nyquist() || amplitude <= 0.f) continue;
        frequency += change;
    }
}

void RepitchRatio::proc()
{
    float change = value.get_value();

    for (int n = 0; n < Cluster::partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = target_cluster->partials[n];
        if (frequency >= var::get_nyquist() || amplitude <= 0.f) continue;
        frequency *= change;
    }
}
}
namespace WaveTransforms
{
PartialIndexTransform Sine
    = [](unsigned n, float fundamental, Partial &p)
{
    p.amplitude = 0.f;
    p.frequency = fundamental;

    if (n == 0) p.amplitude = 1.f;
};

PartialIndexTransform Saw
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 1.f / (n + 1);
};

PartialIndexTransform Square
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 1.f / (n + 1);

    if (n % 2) p.amplitude = 0.f;
};

PartialIndexTransform Tri
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 1.f / pow(n + 1, 2);

    if (n % 2) p.amplitude = 0;
    if ((n+2) % 4 != 0) p.offset_phase = var::tau/2;
};
}

}