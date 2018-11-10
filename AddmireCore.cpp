
#include <cmath>
#include <random>
#include <fstream>

#include "AddmireCore.h"

namespace add {

int Cluster::partials_used;

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
}   }

namespace wavetable
{
    const int table_size = 512;
    namespace { float table[table_size]; }

    float get_value(float phase)
    {
        while(phase >= 1.f) { phase -= 1.f; }
        while(phase < 0.f)  { phase += 1.f; }
        phase *= table_size;

        return table[(int)phase];
}   };

void addmire_init(float sample_rate /*=44100.0*/, int partialc /*=512*/)
{
    var::sample_rate = sample_rate;
    var::nyquist = sample_rate / 2.f;
    Cluster::partials_used = partialc;

    for (int n = 0; n < wavetable::table_size; n++)
        wavetable::table[n] = sinf(var::tau * n / wavetable::table_size);
}

Partial::Partial()
{
    frequency     = 100.f;
    offset_phase  = 0.f;
    amplitude     = 0.f;
    current_phase = 0.f;
}

Cluster::Cluster(float fundamental, PartialIndexTransform transform)
{
    for (unsigned n = 0; n < Cluster::partials_used; n++)
    {
        Partial& p = partials[n];

        transform(n, fundamental, p);

        if (p.frequency >= var::get_nyquist())
            p.amplitude = 0.f;
}   }

void Cluster::get_samples(float* buffer, int buffersize)
{
    for (unsigned n = 0; n < Cluster::partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = partials[n];
        if (frequency >= var::get_nyquist() || amplitude <= 0.f) continue;

        for (unsigned s = 0; s < buffersize; s++)
        {
            buffer[s] += amplitude * wavetable::get_value(phase + offset / var::tau);
            phase += double(frequency) / var::get_sample_rate();
            if (phase >= 1.0) { phase -= 1.0; }
}   }   }

void Cluster::draw()
{
    const int y_res = 1000;
    const int x_res = 250;

    char data[y_res];
    for (unsigned n = 0; n < y_res; n++)
        data[n] = '1';

    for (unsigned n = 0; n < Cluster::partials_used; n++)
    {
        if (partials[n].frequency >= var::get_nyquist()) continue;
        data[int(sqrt(partials[n].frequency) * y_res / sqrt(var::get_nyquist()))] = '0';
    }

    std::fstream file("test_image.pbm", std::fstream::out | std::fstream::binary);

    const char header[] = "P1\n250 1000\n";
    file.write(header, 12);

    for (unsigned y = 1; y <= y_res; y++)
    {
        for (unsigned x = 0; x < x_res; x++)
            file << data[y_res - y] << ' ';

        file << '\n';
}   }

}
