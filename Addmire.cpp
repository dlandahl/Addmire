
#include <cmath>
#include <iostream>

#include "Addmire.h"

namespace add {

namespace var { double sample_rate; }

void addmire_init(double sr /*=44100.0*/)
{
    var::sample_rate = sr;
}

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
    {
        init_partial(&cluster->partials[n]);
    }

    cluster->partials_used = size;
}

void init_cluster_to_wave(Cluster* cluster, float fundamental, WaveType wave)
{
    if (wave == WaveType::Sine)
    {
        cluster->partials_used = 1;
        cluster->partials[0] = make_partial(fundamental);
        return;
    }

    if (wave == WaveType::Saw)
    {
        for (unsigned n = 0; n < Cluster::max_size; n++)
        {
            if (fundamental * (n + 1) >= var::sample_rate / 2)
                cluster->partials_used = n; return;
            
            init_partial(&cluster->partials[n]);
            cluster->partials[n].frequency = (float) fundamental * (n + 1);
            cluster->partials[n].amplitude = (float) 1 / (n + 1);
        }
    }
}

void samples_from_cluster(Cluster* cluster, float* buffer, int buffersize)
{
    for (unsigned p = 0; p < cluster->partials_used; p++)
    {
        Partial &partial = cluster->partials[p];

        for (unsigned s = 0; s < buffersize; s++)
        {
            buffer[s] += std::sin(6.28318530 * partial.current_phase + partial.offset_phase);
            partial.current_phase += double(partial.frequency) / var::sample_rate;
            if (partial.current_phase >= 1.0) { partial.current_phase -= 1.0; }
        }
    }
}

}