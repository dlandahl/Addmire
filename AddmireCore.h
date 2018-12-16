
#pragma once

namespace add {
namespace var {
    inline const float tau = 6.28318530;
    float get_nyquist();
    float get_sample_rate();
    void set_sample_rate(float new_sample_rate);
    void set_partial_count(int new_partial_count);
}

void addmire_init(float sample_rate=44100.0f, int partial_count=512);

namespace wavetable
{
    inline const int table_size = 512;
    float get_value();
}

struct Partial
{
    float frequency;
    float offset_phase;
    float amplitude;
    float current_phase;

    Partial();
};

using PartialIndexTransform = void (*)(unsigned partial_index,
    float fundamental, Partial &partial);

class Cluster
{
public:
    const static int max_size = 4410;
    static int partials_used;

    Partial partials[max_size];

    Cluster(float fundamental, PartialIndexTransform transform);
    Cluster() = default;

    static Cluster from_dft(float* data, unsigned size);
    void get_samples(float* buffer, int sample_count);
    void draw();

};

class AdditiveProcessor
{
protected:
    Cluster* target_cluster;
public:
    virtual void proc() = 0;

    AdditiveProcessor(Cluster* c) : target_cluster(c) { }
    AdditiveProcessor() = delete;
};
}