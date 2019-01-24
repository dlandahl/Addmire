
#include <vector>
#include <cmath>
#include <fstream>
#include <complex>

#include "AddmireCore.h"

namespace add {

int Cluster::partials_used;

namespace var
{
    namespace { float nyquist, sample_rate; }
    
    float get_nyquist()     { return nyquist; }
    float get_sample_rate() { return sample_rate; }

    void set_sample_rate(float sample_rate) {
        var::sample_rate = sample_rate;
        var::nyquist = sample_rate / 2.f;
    }

    void set_partial_count(int partialc) {
        if (partialc <= Cluster::max_size) Cluster::partials_used = partialc;
}   }

namespace wavetable
{
    namespace { float table[table_size]; }

    float get_value(float phase)
    {
        while(phase >= 1.f) { phase -= 1.f; }
        while(phase < 0.f)  { phase += 1.f; }
        phase *= table_size;

        //float mu = phase - std::floor(phase);

        //float y1 = table[(int)std::ceil(phase)];
        //float y2 = table[(int)std::floor(phase)];

        //return y1*(1 - mu) + y2*mu;
        return table[(int)phase];
}   }

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
    for (int n = 0; n < Cluster::partials_used; n++)
    {
        Partial& p = partials[n];

        transform(n, fundamental, p);

        if (p.frequency >= var::get_nyquist())
            p.amplitude = 0.f;
}   }

void Cluster::get_samples(float* buffer, int buffersize)
{
    for (int n = 0; n < Cluster::partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = partials[n];
        if (frequency >= var::get_nyquist() || amplitude <= 0.f) continue;

        for (int s = 0; s < buffersize; s++)
        {
            buffer[s] += amplitude * wavetable::get_value(phase + offset / var::tau);
            phase += frequency / var::get_sample_rate();
            if (phase >= 1.0) { phase -= 1.0; }
}   }   }

void Cluster::draw()
{
    int const y_res = 1000;
    int const x_res = 250;

    char data[y_res];
    for (int n = 0; n < y_res; n++)
        data[n] = '1';

    for (int n = 0; n < Cluster::partials_used; n++)
    {
        auto& [frequency, offset, amplitude, phase] = partials[n];
        if (frequency >= var::get_nyquist()) continue;
        if (amplitude >= 0.001)
            data[int(sqrt(frequency) * y_res / sqrt(var::get_nyquist()))] = '0';
    }

    std::fstream file("test_image.pbm", std::fstream::out | std::fstream::binary);

    char const header[] = "P1\n250 1000\n";
    file.write(header, 12);

    for (int y = 1; y <= y_res; y++)
    {
        for (int x = 0; x < x_res; x++)
            file << data[y_res - y] << ' ';

        file << '\n';
}   }

Cluster Cluster::from_dft(float* data, unsigned size)
{
    Cluster c;
    c.partials_used = size / 2;

    for (unsigned b = 0; b < size / 2; b++)
    {
        auto& [frequency, offset, amplitude, phase] = c.partials[b];
        std::complex<float> complex(0.f, 0.f);

        for (unsigned s = 0; s < size; s++)
        {
            float real = data[s] * sin(var::tau * s * b / size);
            float imag = data[s] * cos(var::tau * s * b / size) * -1.f;

            complex += std::complex<float>(real, imag);
        }
        complex /= size;

        amplitude = sqrt(pow(complex.real(), 2) + pow(complex.imag(), 2));
        offset    = atan(complex.imag() / complex.real());
        frequency = (var::get_sample_rate() / size) * b;
    }

    return c;
}

}
