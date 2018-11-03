
#include <random>

#include "AddmireAlgorithms.h"

using namespace add;

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

namespace AdditiveProcesses
{
AdditiveProcess random_phase
    = [](Cluster* cluster, float* args, unsigned argc)
{
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.f, (args != nullptr) ? args[0] : var::tau);

    for (unsigned n = 0; n < Cluster::partials_used; n++)
        cluster->partials[n].offset_phase += distribution(generator);
};

AdditiveProcess repitch_ratio
    = [](Cluster* cluster, float* args, unsigned argc)
{
    if (args == nullptr) return;
    for (unsigned n = 0; n < Cluster::partials_used; n++)
        cluster->partials[n].frequency *= args[0];
};

AdditiveProcess repitch_hz
    = [](Cluster* cluster, float* args, unsigned argc)
{
    if (args == nullptr) return;
    for (unsigned n = 0; n < Cluster::partials_used; n++)
        cluster->partials[n].frequency += args[0];
};
}
