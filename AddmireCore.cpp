
#include <cmath>
#include <random>
#include <fstream>

#include "AddmireCore.h"

namespace add {

int Cluster::partials_used;

union argument
{
    float  float_arg;
    float* float_ptr_arg;
    AdditiveProcess  process_arg;
    AdditiveProcess* process_ptr_arg;
    Cluster  cluster_arg;
    Cluster* cluster_ptr_arg;
};

namespace var {
    const float tau = 6.28318530;
    namespace { float nyquist, sample_rate; }
    
    float get_nyquist()     { return nyquist; }
    float get_sample_rate() { return sample_rate; }

    void set_sample_rate(float sample_rate) {
        var::sample_rate = sample_rate;
        var::nyquist = sample_rate / 2.f;
    }

    void set_partial_count(int partialc) {
        Cluster::partials_used = partialc;
    }
}

namespace wavetable
{
    const int table_size = 512;
    namespace { float table[table_size]; }

    float get_value(float phase)
    {
        while(phase >= 1.f) { phase -= 1.f; }
        while(phase < 0.f)  { phase += 1.f; }
        phase *= 512;

        return table[(int)phase];
    }
};

void addmire_init(float sample_rate /*=44100.0*/, int partialc /*=512*/)
{
    var::sample_rate = sample_rate;
    var::nyquist = sample_rate / 2.f;
    Cluster::partials_used = partialc;

    for (int n = 0; n < wavetable::table_size; n++)
    {
        wavetable::table[n] = sinf(var::tau * n / wavetable::table_size);
    }
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

        if (partial.frequency >= var::get_nyquist())
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
            buffer[s] += amplitude * wavetable::get_value(phase + offset / var::tau);
            phase += double(frequency) / var::get_sample_rate();
            if (phase >= 1.0) { phase -= 1.0; }
}   }   }

void draw_cluster(Cluster* cluster)
{
    const int y_res = 1000;
    const int x_res = 250;

    char data[y_res];
    for (unsigned n = 0; n < y_res; n++)
        data[n] = '1';

    for (unsigned n = 0; n < Cluster::partials_used; n++)
    {
        if (cluster->partials[n].frequency >= var::get_nyquist()) continue;
        data[int(sqrt(cluster->partials[n].frequency) * y_res / sqrt(var::get_nyquist()))] = '0';
    }

    std::fstream file("test_image.pbm", std::fstream::out | std::fstream::binary);

    const char header[] = "P1\n250 1000\n";
    file.write(header, 12);

    for (unsigned y = 1; y <= y_res; y++)
    {
        for (unsigned x = 0; x < x_res; x++)
            file << data[y_res - y] << ' ';

        file << '\n';
    }
}

}
