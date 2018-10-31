
#include <cmath>
#include <float.h>
#include <iostream>
#include <random>

#include "Addmire.h"

namespace add {

namespace var {
    double sample_rate;
    const float tau = 6.28318530;
}

void addmire_init(double sr /*=44100.0*/)
{
    var::sample_rate = sr;
}

PartialIndexTransform WaveTransforms::Sine
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    amplitude = 0.f;

    if (n == 0) {
        frequency = fundamental;
        amplitude = 1.f;
        return;
    }
    else if (n == 1) frequency = FLT_MAX;
};

PartialIndexTransform WaveTransforms::Saw
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    frequency = (n + 1) * fundamental;
    amplitude = 1.f / (n + 1);
};

PartialIndexTransform WaveTransforms::Square
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    frequency = (2 * n + 1) * fundamental;
    amplitude = 1.f / (2 * n + 1);
};

PartialIndexTransform WaveTransforms::Tri
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    frequency = (2 * n + 1) * fundamental;
    amplitude = pow(-1.f, n) / pow(2 * n + 1, 2);
};

Partial make_partial(float frequency /*=100.f*/, float phase /*=0.f*/, float amplitude /*=1.f*/)
{ return Partial{ frequency, phase, amplitude, 0.f }; }

void init_partial(Partial* partial)
{
    partial->frequency     = 100.f;
    partial->offset_phase  = 0.f;
    partial->amplitude     = 1.f;
    partial->current_phase = 0.f;
}

void init_cluster(Cluster* cluster, int size)
{
    for (unsigned n = 0; n < size; n++)
        init_partial(&cluster->partials[n]);

    cluster->partials_used = size;
}

void init_cluster_to_wave(Cluster* cluster, float fundamental, PartialIndexTransform transform)
{
    for (unsigned n = 0; n < Cluster::max_size; n++)
    {
        Partial& partial = cluster->partials[n];
        init_partial(&partial);

        transform(n, fundamental, partial.frequency, partial.amplitude);

        if (partial.frequency >= var::sample_rate / 2)
        {
            cluster->partials_used = n;
            partial.amplitude = 0.f;
            return;
}   }   }

void samples_from_cluster(Cluster* cluster, float* buffer, int buffersize)
{
    for (unsigned n = 0; n < cluster->partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = cluster->partials[n];

        for (unsigned s = 0; s < buffersize; s++)
        {
            buffer[s] += amplitude * std::sin(6.28318530 * phase + offset);
            phase += double(frequency) / var::sample_rate;
            if (phase >= 1.0) { phase -= 1.0; }
}   }   }

AdditiveProcess random_phase
    = [](Cluster* cluster, float* args, unsigned argc)
{
    static std::default_random_engine generator;
    static std::uniform_real_distribution<float> distribution(0.f, argc ? args[0] : var::tau);

    for (unsigned n = 0; n < cluster->partials_used; n++)
        cluster->partials[n].offset_phase = distribution(generator);
};

}
