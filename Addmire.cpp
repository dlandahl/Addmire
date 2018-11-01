
#include <cmath>
#include <float.h>
#include <iostream>
#include <random>

#include "Addmire.h"

namespace add {

int Cluster::partials_used;

namespace var {
    float sample_rate;
    float nyquist;
    const float tau = 6.28318530;
}

void addmire_init(float sr /*=44100.0*/, int partialc /*=512*/)
{
    var::sample_rate = sr;
    var::nyquist = sr / 2.f;
    Cluster::partials_used = partialc;
}

PartialIndexTransform WaveTransforms::Sine
    = [](unsigned n, float fundamental, float& frequency, float& amplitude)
{
    amplitude = 0.f;
    frequency = fundamental;

    if (n == 0)
        amplitude = 1.f;
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
    for (unsigned n = 0; n < Cluster::partials_used; n++)
    {
        Partial& partial = cluster->partials[n];
        init_partial(&partial);

        transform(n, fundamental, partial.frequency, partial.amplitude);

        if (partial.frequency >= var::nyquist)
            partial.amplitude = 0.f;
}   }

void samples_from_cluster(Cluster* cluster, float* buffer, int buffersize)
{
    for (unsigned n = 0; n < Cluster::partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = cluster->partials[n];
        if (frequency >= var::nyquist || amplitude == 0.f) continue;

        for (unsigned s = 0; s < buffersize; s++)
        {
            buffer[s] += amplitude * std::sin(var::tau * phase + offset);
            phase += double(frequency) / var::sample_rate;
            if (phase >= 1.0) { phase -= 1.0; }
}   }   }

AdditiveProcess random_phase
    = [](Cluster* cluster, float* args, unsigned argc)
{
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.f, (args != nullptr) ? args[0] : var::tau);

    for (unsigned n = 0; n < cluster->partials_used; n++)
        cluster->partials[n].offset_phase += distribution(generator);
};

AdditiveProcess repitch_ratio
    = [](Cluster* cluster, float* args, unsigned argc)
{
    if (args == nullptr) return;
    for (unsigned n = 0; n < cluster->partials_used; n++)
        cluster->partials[n].frequency *= args[0];
};

AdditiveProcess repitch_hz
    = [](Cluster* cluster, float* args, unsigned argc)
{
    if (args == nullptr) return;
    for (unsigned n = 0; n < cluster->partials_used; n++)
        cluster->partials[n].frequency += args[0];
};

}
