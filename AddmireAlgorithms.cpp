
#include <random>

#include "AddmireAlgorithms.h"

namespace add {

namespace WaveTransforms
{
PartialIndexTransform Sine
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    amplitude = 0.f;
    frequency = fundamental;

    if (n == 0) amplitude = 1.f;
};

PartialIndexTransform Saw
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    frequency = (n + 1) * fundamental;
    amplitude = 1.f / (n + 1);
};

PartialIndexTransform Square
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    frequency = (2 * n + 1) * fundamental;
    amplitude = 1.f / (2 * n + 1);
};

PartialIndexTransform Tri
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    frequency = (2 * n + 1) * fundamental;
    amplitude = pow(-1.f, n) / pow(2 * n + 1, 2);
};
}

}