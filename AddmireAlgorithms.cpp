
#include <random>
#include <iostream>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

namespace add {

namespace AdditiveProcesses {

}

namespace WaveTransforms
{
WaveTransform Sine
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 0.f;

    if (n == 0) p.amplitude = 1.f;
};

WaveTransform Saw
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 1.f / (n + 1);
};

WaveTransform Square
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 1.f / (n + 1);

    if (n % 2) p.amplitude = 0.f;
};

WaveTransform Tri
    = [](unsigned n, float fundamental, Partial &p)
{
    p.frequency = (n + 1) * fundamental;
    p.amplitude = 1.f / pow(n + 1, 2);

    if (n % 2) p.amplitude = 0;
    if ((n+2) % 4 != 0) p.offset_phase = var::tau/2;
};
}

}
